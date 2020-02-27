define([
    './S3MCacheFileRenderEntity'
    ], function(
    S3MCacheFileRenderEntity
        ) {
    'use strict';

    let S3MContentFactory = {
        'OSGBFile' : function(options) {
            return new S3MCacheFileRenderEntity(options);
        },
        'OSGBCacheFile' : function(options) {
            return new S3MCacheFileRenderEntity(options);
        }
    };

    return S3MContentFactory;
});
