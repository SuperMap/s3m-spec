define([
    ], function(
        ) {
    'use strict';

    let S3MCompressType = {
        encNONE : 0,
        enrS3TCDXTN : 14,
        enrPVRTPF_PVRTC2 : 19,
        enrPVRTPF_PVRTC : 20,
        enrPVRTPF_PVRTC_4bpp : 21,
        enrPVRTPF_ETC1 : 22
    };
    return Object.freeze(S3MCompressType);
});
