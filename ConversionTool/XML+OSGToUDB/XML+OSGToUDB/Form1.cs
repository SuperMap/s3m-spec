using SuperMap.Data;
using SuperMap.Realspace.ThreeDDesigner;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace XML_OSGToUDB
{
    public partial class Form1 : Form
    {
        Workspace m_workspace = new Workspace();
        public Form1()
        {
            InitializeComponent();
        }

        private void ReadXmlFile(string xmlFile,DatasetVector dv)
        {
            if (xmlFile == String.Empty)
            {
                return;
            }
            Recordset recordset = dv.GetRecordset(false, CursorType.Dynamic);
            recordset.Batch.Begin();
            ModelEntityManager em = new ModelEntityManager(dv);
            XmlDocument doc = new XmlDocument();
            doc.Load(xmlFile);
            //root Node
            XmlNode rootNode = doc.SelectSingleNode("ModelPointClass");

            //
            XmlNodeList ModelPointList = rootNode.SelectNodes("ModelPoint");
            Dictionary<string, List<string>> osgFileAndSkeletons = new Dictionary<string, List<string>>();
            foreach (XmlNode ModelPointNode in ModelPointList)
            {
                XmlNode ModelNameNode = ModelPointNode.SelectSingleNode("ModelName");
                if (!osgFileAndSkeletons.Keys.Contains(ModelNameNode.InnerText))
                    osgFileAndSkeletons.Add(ModelNameNode.InnerText, new List<string>());
            }
            //模型骨架写实体管理器，模型有复用
            foreach (string modelName in osgFileAndSkeletons.Keys)
            {
                Model model = new Model();
                ModelFileSetting modelFileSetting = new ModelFileSetting();
                modelFileSetting.FilePath = Path.Combine(textBox1.Text, modelName + ".osg");
                ModelConvertor.FromFile(model, modelFileSetting);
                for (int i = 0; i < model.GetSkeletonCount(-1); i++)
                {
                    SkeletonID id = new SkeletonID(-1, i);
                    Skeleton skeleton = model.GetSkeleton(id);
                    Skeleton newSkeleton = em.Append(skeleton);
                    osgFileAndSkeletons[modelName].Add(newSkeleton.Name);
                    newSkeleton.Dispose();
                    skeleton.Dispose();
                }
                model.Dispose();
            }
            em.Save();
            //写GeoModel3D
            foreach (XmlNode ModelPointNode in ModelPointList)
            {
                XmlNode ModelNameNode = ModelPointNode.SelectSingleNode("ModelName");
                XmlNode LocationXNode = ModelPointNode.SelectSingleNode("LocationX");
                XmlNode LocationYNode = ModelPointNode.SelectSingleNode("LocationY");
                XmlNode LocationZNode = ModelPointNode.SelectSingleNode("LocationZ");
                XmlNode Matrix3Node = ModelPointNode.SelectSingleNode("Matrix3");
                string modelName = ModelNameNode.InnerText;
                string[]temp= Matrix3Node.InnerText.Split(',');
                GeoModel3D model3d = new GeoModel3D();
                Model model = new Model();
                //3*3的矩阵转为4*4的矩阵
                double[] matrix = new double[] {
                    double.Parse(temp[0],NumberStyles.Any),double.Parse(temp[1],NumberStyles.Any),double.Parse(temp[2],NumberStyles.Any),0,
                    double.Parse(temp[3],NumberStyles.Any),double.Parse(temp[4],NumberStyles.Any),double.Parse(temp[5],NumberStyles.Any),0,
                    double.Parse(temp[6],NumberStyles.Any),double.Parse(temp[7],NumberStyles.Any),double.Parse(temp[8],NumberStyles.Any),0,
                    0,0,0,1
                };
                //从实体管理器构建Model
                foreach (string skeletonName in osgFileAndSkeletons[modelName])
                {
                    model.AddSkeleton(em, skeletonName, matrix);
                }
                model3d.Model = model;
                model3d.Position = new Point3D(double.Parse(LocationXNode.InnerText), double.Parse(LocationYNode.InnerText), double.Parse(LocationZNode.InnerText));
                if(dv.HasPrjCoordSys)
                {
                    if(dv.PrjCoordSys.CoordUnit==Unit.Degree)
                    {
                        model3d.IsLonLat = true;
                    }
                    else
                    {
                        model3d.IsLonLat = false;
                    }
                }
                else
                {
                    model3d.IsLonLat = false;
                }
                recordset.AddNew(model3d);
            }
            osgFileAndSkeletons.Clear();
            recordset.Batch.Update();
            recordset.Dispose();
        }

        private Datasource CreateDatasource(string udbPath)
        {
            DatasourceConnectionInfo con = new DatasourceConnectionInfo();
            con.Server = udbPath;
            con.EngineType = EngineType.UDB;
            return m_workspace.Datasources.Create(con);
        }

        private DatasetVector CreateDatasetVector(Datasource datasource,string datasetName,DatasetType type,bool isLonlat)
        {
            DatasetVectorInfo dvInfo = new DatasetVectorInfo(); 
            dvInfo.Name = datasource.Datasets.GetAvailableDatasetName(datasetName);
            dvInfo.Type = type;
            DatasetVector dv = datasource.Datasets.Create(dvInfo);
            if(isLonlat)
                dv.PrjCoordSys = new PrjCoordSys(PrjCoordSysType.EarthLongitudeLatitude);
            return dv;
        }

        private void button_1_Click(object sender, EventArgs e)
        {
            if(folderBrowserDialog1.ShowDialog()==DialogResult.OK)
            {
                textBox1.Text = folderBrowserDialog1.SelectedPath;
            }
        }

        private void button_2_Click(object sender, EventArgs e)
        {
            saveFileDialog1.Filter = "(*.udb)|*.udb";
            if(saveFileDialog1.ShowDialog()==DialogResult.OK)
            {
                textBox2.Text = saveFileDialog1.FileName;
            }
        }

        private void button_OK_Click(object sender, EventArgs e)
        {

            if (textBox1.Text == "" || textBox2.Text == "")
                return;
            button_OK.Enabled = false;
            Datasource datasource = CreateDatasource(textBox2.Text);
            if (datasource == null)
            {
                button_OK.Enabled = true;
                MessageBox.Show("数据源创建失败，请重试！");
                return;
            }
            string[]xmlFiles= Directory.GetFiles(textBox1.Text, "*.xml", SearchOption.TopDirectoryOnly);
            foreach (string xmlFile in xmlFiles)
            {
                DatasetVector dv= CreateDatasetVector(datasource, Path.GetFileNameWithoutExtension(xmlFile), DatasetType.Model, checkBox1.Checked);
                ReadXmlFile(xmlFile, dv);
                dv.Close();
            }
            datasource.Close();
            button_OK.Enabled = true;
            MessageBox.Show("导出完成！"); 
        }
    }
}
