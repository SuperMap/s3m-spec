import S3MObliqueRenderEntity from './S3MObliqueRenderEntity.js';
import S3MCacheFileRenderEntity from './S3MCacheFileRenderEntity.js';
import S3MGaussianRenderEntity from '../GaussianSplats3D/S3MGaussianRenderEntity.js';

let S3MContentFactory = {
    'OSGBFile' : function(options) {
        return new S3MObliqueRenderEntity(options);
    },
    'OSGBCacheFile' : function(options) {
        return new S3MCacheFileRenderEntity(options);
    },
    // 高斯点云支持
    'PointCloudFile': function(options) {
        return new S3MGaussianRenderEntity(options);
    }
};

export default S3MContentFactory;