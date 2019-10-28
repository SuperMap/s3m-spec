define([
    ], function(
) {
    'use strict';

    let ContentState = {
        UNLOADED : 0,
        LOADING : 1,
        PARSING : 2,
        READY : 3,
        FAILED : 4
    };

    return Object.freeze(ContentState);
});
