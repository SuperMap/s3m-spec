#ifndef UGMARKUP_H
#define UGMARKUP_H

#include "Stream/ugdefs.h"
#include "Stream/ugplatform.h"
#include "Toolkit/UGToolkit.h"

#define EOL _U("\r\n") 
#ifdef _UGUNICODE
#define EOLLEN UGstrlen(EOL)
#else

#define EOLLEN (sizeof(EOL)-1); 
#endif

namespace UGC {

//#ifdef DEBUG
#define _DS(i) (i?&(m_strDoc.Cstr())[m_pos[i].nStartL]:0)
#define MARKUP_SETDEBUGSTATE m_pMainDS=_DS(m_nPos); m_pChildDS=_DS(m_nPosChild)
//#else
	//#ifdef SOLARIS  //added by xielin solaris下release也要这个玩意，要不就崩溃了
	//modified by xielin 2008.8.14 用系统宏sparc保险，SOLARIS宏是自己定义的，如果有一个编辑脚本忘记加了，就崩溃了，切记
// #ifdef sparc
// #define _DS(i) (i?&(m_strDoc.c_str())[m_pos[i].nStartL]:0)
// #define MARKUP_SETDEBUGSTATE m_pMainDS=_DS(m_nPos); m_pChildDS=_DS(m_nPosChild)
// #else
// #define MARKUP_SETDEBUGSTATE
// #endif
// #endif

//XML文件头的格式。
#define XML_FILE_HEADER  _U("<?xml version=\"%VERSION\" encoding=\"%ENCODING\"?>")
//! \brief XML解析类
//! \remarks 从网上下载的一个类, 原名为UGMarkup; 详细的功能使用说明请参考最后的注释说明. 
//! UGMarkup是基于“压缩”文档对象模型(EDOM ： "Encapsulated" Document Object Model,)，
//!	这是这个简单XML处理器的关键。它的一套XML处理方法和产生的效果与DOM(Document Object Model)
//!	是一样的。但是整个DOM有无数的对象类型，而EDOM只定义了一个对象：XML文档，EDOM回到了XML原始的吸引力—简单. 
//! Markup 封装了XML文档文本、结构和当前位置。它提供了增加元素、导航和得到元素属性和数据的所有方法。
//! 在文档中操作被执行的地方取决于当前位置和当前子位置。当前位置允许你增加一个元素到文档中的这个位置，
//! 而不需要明确指出增加对象到文档的这点上，在Markup的有效生命周期里，
//! 对象一直保持着一个用来描述文档的字符串，当你调用GetDoc.可以得到它.
class TOOLKIT_API UGMarkup
{
public:
	//! \brief
	UGMarkup();
	//! \brief
	UGMarkup( const UGchar* szDoc ); 
	//! \brief
	UGMarkup( const UGMarkup& markup ) { *this = markup; };
	//! \brief
	void operator=( const UGMarkup& markup );
	//! \brief
	~UGMarkup();

	// Navigate
	//加载的时候会将XML文件中的编码方式，转换到当前的默认编码方式
	UGbool Load( const UGchar* szFileName );
	//! \brief
	UGbool SetDoc( const UGchar* szDoc );
	//! \brief
	UGbool IsWellFormed();
	//! \brief
	UGbool FindElem( const UGchar* szName=NULL );
	//! \brief
	UGbool FindChildElem( const UGchar* szName=NULL );
	//! \brief
	UGbool IntoElem();
	//! \brief
	UGbool OutOfElem();
	//! \brief
	void ResetChildPos() { SetPos(m_posParent,m_nPos,0); };
	//! \brief
	void ResetMainPos() { SetPos(m_posParent,0,0); };
	//! \brief
	void ResetPos() { SetPos(0,0,0); };
	//! \brief
	UGString GetTagName() const;
	//! \brief
	UGString GetChildTagName() const { return GetTagName(m_nPosChild); };
	//! \brief
	UGString GetData() const { return GetData(m_nPos); };
	//! \brief
	UGString GetChildData() const { return GetData(m_nPosChild); };
	//! \brief
	UGString GetAttrib( const UGchar* szAttrib ) const { return GetAttrib(m_nPos,szAttrib); };
	//! \brief
	UGString GetChildAttrib( const UGchar* szAttrib ) const { return GetAttrib(m_nPosChild,szAttrib); };
	//! \brief
	UGString GetAttribName( UGint n ) const;
	//! \brief
	UGString GetChildAttribName( UGint n ) const;
	//! \brief
	UGbool SavePos( const UGchar* szPosName=_U("") );
	//! \brief
	UGbool RestorePos( const UGchar* szPosName=_U("") );
	//! \brief
	const UGString& GetError() const { return m_strError; };
	//! \brief

	//! \brief
	UGbool MoveNextElem();
	//! \brief
	UGbool MoveNextChildElem();
	//! \brief
	UGbool HasChild() const;
	//! \brief
	UGint GetAttribCount() const;
	//! \brief
	UGint GetChildAttribCount() const;
	//! \brief

	//! \brief
	enum MarkupNodeType
	//! \brief
	{
	//! \brief
		MNT_ELEMENT					= 1,  // 0x01
	//! \brief
		MNT_TEXT					= 2,  // 0x02
	//! \brief
		MNT_WHITESPACE				= 4,  // 0x04
	//! \brief
		MNT_CDATA_SECTION			= 8,  // 0x08
	//! \brief
		MNT_PROCESSING_INSTRUCTION	= 16, // 0x10
	//! \brief
		MNT_COMMENT					= 32, // 0x20
	//! \brief
		MNT_DOCUMENT_TYPE			= 64, // 0x40
	//! \brief
		MNT_EXCLUDE_WHITESPACE		= 123// 0x7b
	//! \brief
	//! \brief
	};
	//! \brief

	// Create
	//保存的时候会根据你设置的编码方式保存当前的XML的文件
	//! \brief
	UGbool Save( const UGchar* szFileName );
	//! \brief
	const UGString& GetDoc() const { return m_strDoc; };
	//! \brief
	UGbool AddElem( const UGchar* szName, const UGchar* szData=NULL ) { return AddElem(szName,szData,false,false); };
	//! \brief
	UGbool InsertElem( const UGchar* szName, const UGchar* szData=NULL ) { return AddElem(szName,szData,true,false); };
	//! \brief
	UGbool AddChildElem( const UGchar* szName, const UGchar* szData=NULL ) { return AddElem(szName,szData,false,true); };
	//! \brief
	UGbool InsertChildElem( const UGchar* szName, const UGchar* szData=NULL ) { return AddElem(szName,szData,true,true); };
	//! \brief
	UGbool AddAttrib( const UGchar* szAttrib, const UGchar* szValue ) { return SetAttrib(m_nPos,szAttrib,szValue); };
	//! \brief
	UGbool AddChildAttrib( const UGchar* szAttrib, const UGchar* szValue ) { return SetAttrib(m_nPosChild,szAttrib,szValue); };
	//! \brief
	UGbool AddAttrib( const UGchar* szAttrib, UGint nValue ) { return SetAttrib(m_nPos,szAttrib,nValue); };
	//! \brief
	UGbool AddChildAttrib( const UGchar* szAttrib, UGint nValue ) { return SetAttrib(m_nPosChild,szAttrib,nValue); };
	//! \brief
	UGbool AddSubDoc( const UGchar* szSubDoc ) { return AddSubDoc(szSubDoc,false,false); };
	//! \brief
	UGbool InsertSubDoc( const UGchar* szSubDoc ) { return AddSubDoc(szSubDoc,true,false); };
	//! \brief
	UGString GetSubDoc() const { return GetSubDoc(m_nPos); };
	//! \brief
	UGbool AddChildSubDoc( const UGchar* szSubDoc ) { return AddSubDoc(szSubDoc,false,true); };
	//! \brief
	UGbool InsertChildSubDoc( const UGchar* szSubDoc ) { return AddSubDoc(szSubDoc,true,true); };
	//! \brief
	UGString GetChildSubDoc() const { return GetSubDoc(m_nPosChild); };
	//! \brief

	//! \brief
	// Modify
	//! \brief
	UGbool RemoveElem();
	//! \brief
	UGbool RemoveChildElem();
	//! \brief
	UGbool SetAttrib( const UGchar* szAttrib, const UGchar* szValue ) { return SetAttrib(m_nPos,szAttrib,szValue); };
	//! \brief
	UGbool SetChildAttrib( const UGchar* szAttrib, const UGchar* szValue ) { return SetAttrib(m_nPosChild,szAttrib,szValue); };
	//! \brief
	UGbool SetAttrib( const UGchar* szAttrib, UGint nValue ) { return SetAttrib(m_nPos,szAttrib,nValue); };
	//! \brief
	UGbool SetChildAttrib( const UGchar* szAttrib, UGint nValue ) { return SetAttrib(m_nPosChild,szAttrib,nValue); };
	//! \brief
	UGbool SetData( const UGchar* szData, UGint nCDATA=0 ) { return SetData(m_nPos,szData,nCDATA); };
	//! \brief
	UGbool SetRealData( const UGchar* szData ) { return SetRealData(m_nPos,szData); };
	//! \brief
	UGbool SetChildData( const UGchar* szData, UGint nCDATA=0 ) { return SetData(m_nPosChild,szData,nCDATA); };
	//! \brief
    
	//! \brief
	//! \brief 根据当前的编码方式获取的XML文件头
	UGString GetXMLFileHeader();
	//! \brief 获取当前XML的编码方式
	UGMBString::Charset GetEncoding();
	//! \brief 设置当前的编码方式，则XML保存的时候会将当前系统默认的编码方式保存到这个编码方式

	void SetEncoding(UGMBString::Charset nEncoding);


protected:

//#ifdef DEBUG
	const UGchar* m_pMainDS;
	const UGchar* m_pChildDS;
//#endif

	UGMBString::Charset m_nEncoding;
	UGString m_strDoc;
	UGString m_strError;
//	UGint m_nFlags;

	struct ElemPos
	{
		ElemPos() { Clear(); };
		ElemPos( const ElemPos& pos ) { *this = pos; };
		UGbool IsEmptyElement() const { return (nStartR == nEndL + 1); };
		void Clear() { memset( this, 0, sizeof(ElemPos) ); };
		void AdjustStart( UGint n ) { nStartL+=n; nStartR+=n; };
		void AdjustEnd( UGint n ) { nEndL+=n; nEndR+=n; };
		UGint nStartL;
		UGint nStartR;
		UGint nEndL;
		UGint nEndR;
		UGint nReserved;
		UGint iElemParent;
		UGint iElemChild;
		UGint iElemNext;
	};

	UGint m_posParent;
	UGint m_nPos;
	UGint m_nPosChild;
	UGint m_nPosFree;
	UGint m_nNodeType;

	struct TokenPos
	{
		TokenPos( const UGchar* sz ) { Clear(); szDoc = sz; };
		void Clear() { nL=0; nR=-1; nNext=0; bIsString=false; };
		UGbool Match( const UGchar* szName )
		{
			UGint nLen = nR - nL + 1;
		// To ignore case, define MARKUP_IGNORECASE
		#ifdef MARKUP_IGNORECASE
			return ( (UGstrnicmp( &szDoc[nL], szName, nLen ) == 0)
		#else
			return ( (UGstrncmp( &szDoc[nL], szName, nLen ) == 0)
		#endif
				&& ( szName[nLen] == _U('\0') || UGstrchr(_U(" =//["),szName[nLen]) ) );
		};
		UGint nL;
		UGint nR;
		UGint nNext;
		const UGchar* szDoc;
		UGbool bIsString;
	};

	struct SavedPos
	{
		UGint iPosParent;
		UGint iPos;
		UGint iPosChild;
	};
	typedef std::map<UGString,SavedPos> mapSavedPosT;
	mapSavedPosT m_mapSavedPos;

	void SetPos( UGint iPosParent, UGint iPos, UGint iPosChild )
	{
		m_posParent = iPosParent;
		m_nPos = iPos;
		m_nPosChild = iPosChild;
		m_nNodeType = iPos?MNT_ELEMENT:0;
		MARKUP_SETDEBUGSTATE;
	};

	struct PosArray
	{
	  //!  \brief Toolkit
		PosArray() { Clear(); };
	  //!  \brief Toolkit
		~PosArray() { Release(); };
	  //!  \brief Toolkit
		enum { PA_SEGBITS = 16, PA_SEGMASK = 0xffff };
	  //!  \brief Toolkit
		void RemoveAll() { Release(); Clear(); };
	  //!  \brief Toolkit
		void Release() { 
			for (UGint n=0;n<SegsUsed();++n) 
				delete[] (UGchar*)pSegs[n]; 
			if (pSegs) 
				delete[] (UGchar*)pSegs; };
	  //!  \brief Toolkit
		void Clear() { nSegs=0; nSize=0; pSegs=NULL; };
	  //!  \brief Toolkit
		UGint GetSize() const { return nSize; };
	  //!  \brief Toolkit
		UGint SegsUsed() const { return ((nSize-1)>>PA_SEGBITS) + 1; };
	  //!  \brief Toolkit
		ElemPos& operator[](UGint n) const { return pSegs[n>>PA_SEGBITS][n&PA_SEGMASK]; };
	  //!  \brief Toolkit
		ElemPos** pSegs;
	  //!  \brief Toolkit
		UGint nSize;
	  //!  \brief Toolkit
		UGint nSegs;
	};
	PosArray m_pos;

	  //!  \brief Toolkit
	UGint GetFreePos() { if (m_nPosFree==m_pos.GetSize()) AllocPosArray(); return m_nPosFree++; };
	  //!  \brief Toolkit
	UGint ReleasePos() { --m_nPosFree; return 0; };
	  //!  \brief Toolkit
	UGbool AllocPosArray( UGint nNewSize = 0 );
	  //!  \brief Toolkit
	UGbool ParseDoc();
	  //!  \brief Toolkit
	UGint ParseElem( UGint iPos );
	  //!  \brief Toolkit
	UGint ParseError( const UGchar* szError, const UGchar* szName = NULL );
	  //!  \brief Toolkit
	static UGbool FindChar( const UGchar* szDoc, UGint& nChar, UGchar c );
	  //!  \brief Toolkit
	static UGbool FindAny( const UGchar* szDoc, UGint& nChar );
	  //!  \brief Toolkit
	static UGbool FindToken( TokenPos& token );
	  //!  \brief Toolkit
	UGString GetToken( const TokenPos& token ) const;
	  //!  \brief Toolkit
	UGint FindElem( UGint iPosParent, UGint iPos, const UGchar* szPath );
	  //!  \brief Toolkit
	UGString GetTagName( UGint iPos ) const;
	  //!  \brief Toolkit
	UGString GetData( UGint iPos ) const;
	  //!  \brief Toolkit
	UGString GetAttrib( UGint iPos, const UGchar* szAttrib ) const;
	  //!  \brief Toolkit
	UGbool AddElem( const UGchar* szName, const UGchar* szValue, UGbool bInsert, UGbool bAddChild );
	  //!  \brief Toolkit
	UGString GetSubDoc( UGint iPos ) const;
	  //!  \brief Toolkit
	UGbool AddSubDoc( const UGchar* szSubDoc, UGbool bInsert, UGbool bAddChild );
	  //!  \brief Toolkit
	UGbool FindAttrib( TokenPos& token, const UGchar* szAttrib=NULL ) const;
	  //!  \brief Toolkit
	UGbool SetAttrib( UGint iPos, const UGchar* szAttrib, const UGchar* szValue );
	  //!  \brief Toolkit
	UGbool SetAttrib( UGint iPos, const UGchar* szAttrib, UGint nValue );
	  //!  \brief Toolkit
	UGbool CreateNode( UGString& strNode, UGint nNodeType, const UGchar* szText );
	  //!  \brief Toolkit
	void LocateNew( UGint iPosParent, UGint& iPosRel, UGint& nOffset, UGint nLength, UGint nFlags );
	  //!  \brief Toolkit
	UGint ParseNode( TokenPos& token );
	  //!  \brief Toolkit
	UGbool SetData( UGint iPos, const UGchar* szData, UGint nCDATA );
	  //!  \brief Toolkit
	UGbool SetRealData( UGint iPos, const UGchar* szData );
	  //!  \brief Toolkit
	UGint RemoveElem( UGint iPos );
	  //!  \brief Toolkit
	void DocChange( UGint nLeft, UGint nReplace, const UGString& strInsert );
	  //!  \brief Toolkit
	void PosInsert( UGint iPos, UGint nInsertLength );
	  //!  \brief Toolkit
	void Adjust( UGint iPos, UGint nShift, UGbool bAfterPos = false );
	  //!  \brief Toolkit
	UGString TextToDoc( const UGchar* szText, UGbool bAttrib = false ) const;
	  //!  \brief Toolkit
	UGString TextFromDoc( UGint nLeft, UGint nRight ) const;
	  //!  \brief Toolkit
};
	  //!  \brief Toolkit

}

#endif

/** Markup 的主要功能使用说明:
1, 创建一个XML文档
对于创建一个XML文档，需要实例化一个UGMarkup对象，并调用AddElem创建根元素。
在这个位置，如果你调用 AddElem("ORDER") ，你的文档会简单的装一个空ORDER元素<ORDER/>. 
然后调用AddChildElem 在根元素的下面创建元素 (例如：“进入”根元素内部，层次表示).
下面的示例代码创建一个XML文档并返回它（的内容）到一个字符串中。
	  //!  \brief Toolkit
UGMarkup xml;
	  //!  \brief Toolkit
xml.AddElem( "ORDER" );
	  //!  \brief Toolkit
xml.AddChildElem( "ITEM" );
	  //!  \brief Toolkit
xml.IntoElem();
	  //!  \brief Toolkit
xml.AddChildElem( "SN", "132487A-J" );
	  //!  \brief Toolkit
xml.AddChildElem( "NAME", "crank casing" );
	  //!  \brief Toolkit
	  //!  \brief Toolkit
xml.AddChildElem( "QTY", "1" );
	  //!  \brief Toolkit
CString csXML = xml.GetDoc();
这些代码产生了下面的XML，这个根结点是ORDER元素；注意它的开始标签<ORDER> 在开头，结束标签</ORDER>在结尾。
当一个元素是在一个父下面（深入或被包含），这个父元素的开始标签要在它之前，结束标签要在它之后。
ORDER元素包含一个ITEM元素，而ITEM元素包含了三个字子元素：SN、NAME和QTY；
<ORDER>
	<ITEM>
		<SN>132487A-J</SN>
		<NAME>crank casing</NAME>
		<QTY>1</QTY>
	</ITEM>
</ORDER>
如例子中所显示的，你也能够在一个子元素下创建新元素，这需要调用IntoElem 移动你的当前主位置到当前子元素位置，
然后你就可以在这下面增加一个子元素了。UGMarkup在索引中保持了一个当前位置指针，以保证你的源码更加短和更简单，
当导航文件时，相同的逻辑位置也会被使用。

2,导航XML文档
上面的例子所创建的XML字符串，用SetDoc方法加入到UGMarkup对象中能够被解析，
你也可以引导它正确的进入被创建的同一个UGMarkup对象中，如果你要设置当前位置到文档的开始时，需要调用ResetPos.
在下面的例子中，从csXML字符串生成UGMarkup对象后，我们循环ORDER元素下的所有ITEM元素，并得到每个项目的序号和数量。
UGMarkup xml;
	  //!  \brief Toolkit
xml.SetDoc( csXML );
	  //!  \brief Toolkit
while ( xml.FindChildElem("ITEM") )
	  //!  \brief Toolkit
{
	  //!  \brief Toolkit
    xml.IntoElem();
	  //!  \brief Toolkit
    xml.FindChildElem( "SN" );
	  //!  \brief Toolkit
    CString csSN = xml.GetChildData();
	  //!  \brief Toolkit
    xml.FindChildElem( "QTY" );
	  //!  \brief Toolkit
    UGint nQty = atoi( xml.GetChildData() );
	  //!  \brief Toolkit
    xml.OutOfElem();
	  //!  \brief Toolkit
}
对于我们发现的每个元素，在查询它了子元素之前要调用IntoElem，查询完之后再调用OutOfElem ，
当你习惯于这种导航类型时，你将知道，检查你的循环时，要确定每个IntoElem 调用都有一个与之对应的OutOfElem 调用 。

3,增加元素和属性
上面创建文档的例子中仅创建了一个ITEM元素，现在这个例子是创建多个项目，从前一个内容加裁后，再增加数据源，
加上SHIPMENT信息元素中有一个属性，这段代码也演示了你能调用调用IntoElem和AddElem来代替AddChildElem,函数调用。
虽然这意味着更多的调用，但许多人认为这样更直观。
UGMarkup xml;
	  //!  \brief Toolkit
xml.AddElem( "ORDER" );
	  //!  \brief Toolkit
xml.IntoElem(); // inside ORDER
	  //!  \brief Toolkit
for ( UGint nItem=0; nItem<aItems.GetSize(); ++nItem )
	  //!  \brief Toolkit
{
	  //!  \brief Toolkit
    xml.AddElem( "ITEM" );
	  //!  \brief Toolkit
    xml.IntoElem(); // inside ITEM
	  //!  \brief Toolkit
    xml.AddElem( "SN", aItems[nItem].csSN );
	  //!  \brief Toolkit
    xml.AddElem( "NAME", aItems[nItem].csName );
	  //!  \brief Toolkit
    xml.AddElem( "QTY", aItems[nItem].nQty );
	  //!  \brief Toolkit
    xml.OutOfElem(); // back out to ITEM level
	  //!  \brief Toolkit
}
	  //!  \brief Toolkit
xml.AddElem( "SHIPMENT" );
	  //!  \brief Toolkit
xml.IntoElem(); // inside SHIPMENT
	  //!  \brief Toolkit
xml.AddElem( "POC" );
	  //!  \brief Toolkit
xml.SetAttrib( "type", csPOCType );
	  //!  \brief Toolkit
xml.IntoElem(); // inside POC
	  //!  \brief Toolkit
xml.AddElem( "NAME", csPOCName );
	  //!  \brief Toolkit
xml.AddElem( "TEL", csPOCTel );
	  //!  \brief Toolkit
这段代码产生了下面的XML，根元素ORDER包含两个ITEM元素和一个SHIPMENT元素，
	  //!  \brief Toolkit
ITEM元素全都包含SN、NAME、和QTY元素，
SHIPMENT元素包含一个带有属性类型的POC元素，和NAME及TEL子元素。
<ORDER>
	<ITEM>
		<SN>132487A-J</SN>
		<NAME>crank casing</NAME>
		<QTY>1</QTY>
	</ITEM>
	<ITEM>
		<SN>4238764-A</SN>
		<NAME>bearing</NAME>
		<QTY>15</QTY>
	</ITEM>
	<SHIPMENT>
		<POC type="non-emergency">
			<NAME>John Smith</NAME>
			<TEL>555-1234</TEL>
		</POC>
	</SHIPMENT>
</ORDER>

4, 查找元素
FindElem 和 FindChildElem方法用于到下一个兄弟元素。如果可选的标签名被指定，
那么它们将到下一个与标签名相匹配的元素，被发现的元素是当前元素，
并且下次调用Find将会到当前位置后的下一个兄弟或下一个匹配兄弟。
当你无法判断元素的索引时，在调用两个Find方法之间，一定要复位当前位置。
看上面的例子中ITEM元素，如果是别的人创建的XML文件，你不能确定SN元素在QTY元素之前，
那么在查找QTY元素之前就要调用ResetChildPos();
对于用一个特定的序号去查找元素，你需要完全循环ITEM元素，并比较SN元素的数据和你正在搜索的序号。
这个例子不同于先前导航的例子，它调用IntoElem 进入到ORDER元素，
并且用FindElem("ITEM")替换FindChildElem("ITEM");其实两种方式都挺好。
需要注意的是，在Find方法中指定ITEM元素的标签名，我们会忽略所有其它的兄弟元素，例如SHIPMENT元素。
	  //!  \brief Toolkit
UGMarkup xml;
	  //!  \brief Toolkit
xml.SetDoc( csXML );
	  //!  \brief Toolkit
xml.FindElem(); // ORDER element is root
	  //!  \brief Toolkit
xml.IntoElem(); // inside ORDER
	  //!  \brief Toolkit
while ( xml.FindElem("ITEM") )
	  //!  \brief Toolkit
{
	  //!  \brief Toolkit
    xml.FindChildElem( "SN" );
	  //!  \brief Toolkit
    if ( xml.GetChildData() == csFindSN )
	  //!  \brief Toolkit
        break; // found
	  //!  \brief Toolkit
}

5,编码
ASCII编码引用了我们所依靠的字符码128以下的字符，如用英语编程。
如果你只使用ASCII码，很方便，UTF-8编程与你拉公共ASCII集相同。
如果你所使用的字符集不在Unicode编码集(UTF-8，UTF-16，UCS-2)中，
那么出于交互性以及在IE中很好的显示，你真的需要在XML声明中进行描述。
像ISO-8859-1(西欧)字符集指定字符值在一个比特且在128到255之间。
以便每个字符仍然使用一个比特。Windows双字节字符集像GB2312，Shift_JIS和EUC-KR，
每个字符都是用一个或两个字节，对于这些Windows字符集，在你的预处理中需要定义 _MBCS ，
并要确定用户的操作系统设置到合适的编码页。
关于用一个XML描述的XML文档前缀，像<?xml version="1.0" encoding="ISO-8859-1"?>，
需要通过用SetDoc或UGMarkup的构造函数来传递。在结尾要包括回车符，这样根结点会显示在下一行。
	  //!  \brief Toolkit
xml.SetDoc( "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\r\n" );
	  //!  \brief Toolkit
xml.AddElem( "island", "Cura?ao" );

*/


