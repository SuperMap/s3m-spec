import S3MCacheFileRenderEntity from './S3MCacheFileRenderEntity.js';
let S3MContentFactory = {
    'OSGBCacheFile' : function(options) {
        return new S3MCacheFileRenderEntity(options);
    }
};

export default S3MContentFactory;