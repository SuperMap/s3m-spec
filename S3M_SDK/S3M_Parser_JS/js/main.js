var developMode = true;

if (developMode) {
    require.config({
        baseUrl: './Source'
    });
} else {
    require.config({
        waitSeconds: 600,
        paths: {
            'Cesium': '../Build/Cesium/Cesium'
        },
        shim: {
            Cesium: {
                exports: 'Cesium'
            }
        }
    });
}

if (typeof Cesium !== "undefined") {
    onload(Cesium);
} else if (typeof require === "function") {
    require(["Cesium"], onload);
}
