import ContentState from './Enum/ContentState.js';
function S3MLayerScheduler (){
    this._stack = [];
    this._selectPageLods = [];
}

function loadTile(layer, tile, frameState) {
    if(tile.requestedFrame === frameState.frameNumber || tile.contentState !== ContentState.UNLOADED) {
        return ;
    }

    layer._requestTiles.push(tile);
    tile.requestedFrame = frameState.frameNumber;
}

function processTile(layer, tile, frameState) {
    if(tile.processFrame === frameState.frameNumber) {
        return ;
    }

    tile.processFrame = frameState.frameNumber;
    layer._processTiles.push(tile);
}

function touchTile(layer, tile, frameState) {
    if (tile.touchedFrame === frameState.frameNumber) {
        return;
    }

    layer._cache.touch(tile);
    tile.touchedFrame = frameState.frameNumber;
}

function updateMinimumMaximumPriority(layer, tile) {
    layer._maximumPriority.distance = Math.max(tile.distanceToCamera, layer._maximumPriority.distance);
    layer._minimumPriority.distance = Math.min(tile.distanceToCamera, layer._minimumPriority.distance);
    layer._maximumPriority.depth = Math.max(tile.depth, layer._maximumPriority.depth);
    layer._minimumPriority.depth = Math.min(tile.depth, layer._minimumPriority.depth);
    layer._maximumPriority.foveatedFactor = Math.max(tile.foveatedFactor, layer._maximumPriority.foveatedFactor);
    layer._minimumPriority.foveatedFactor = Math.min(tile.foveatedFactor, layer._minimumPriority.foveatedFactor);
    layer._maximumPriority.pixel = Math.max(tile.pixel, layer._maximumPriority.pixel);
    layer._minimumPriority.pixel = Math.min(tile.pixel, layer._minimumPriority.pixel);
}

function updateTile(frameState, layer, tile) {
    if (tile.updatedVisibilityFrame !== frameState.frameNumber) {
        tile.updatedVisibilityFrame = frameState.frameNumber;
        tile.updateVisibility(frameState, layer);
    }

    updateMinimumMaximumPriority(layer, tile);
}

function updatePriority(layer, frameState) {
    let requestTiles = layer._requestTiles;
    let length = requestTiles.length;
    for (let i = 0; i < length; ++i) {
        requestTiles[i].updatePriority(layer, frameState);
    }
}

S3MLayerScheduler.prototype.schedule = function(layer, frameState) {
    this._selectPageLods.length = 0;
    this._stack.length = 0;
    const selectPagelods = this._selectPageLods;
    const stack = this._stack;

    for(let i = 0,j = layer._rootTiles.length;i < j;i++){
        const rootTile = layer._rootTiles[i];
        updateTile(frameState, layer, rootTile);

        if(!rootTile.visible){
            continue ;
        }

        touchTile(layer, rootTile, frameState);

        if(rootTile.contentState === ContentState.UNLOADED){
            loadTile(layer, rootTile, frameState);
            continue ;
        }

        if(rootTile.contentState === ContentState.LOADED) {
            processTile(layer, rootTile, frameState);
            continue ;
        }

        if(rootTile.contentState === ContentState.READY) {
            stack.push(rootTile);
        }

    }

    while(stack.length) {
        const tile = stack.pop();
        touchTile(layer, tile, frameState);

        for(let i = 0,j = tile.pageLods.length;i < j;i++){
            const pageLod = tile.pageLods[i];
            if(pageLod.isLeafTile){
                selectPagelods.push(pageLod);
                continue ;
            }

            pageLod.update(frameState, layer);

            if(!pageLod.canRefine){
                selectPagelods.push(pageLod);
                continue ;
            }

            let childTile = pageLod.childTile;
            if(childTile === undefined){
                pageLod.createChildTile(layer, tile);
                childTile = pageLod.childTile;
                touchTile(layer, childTile, frameState);
                selectPagelods.push(pageLod);
                continue ;
            }

            updateTile(frameState, layer, childTile);
            if(!childTile.visible) {
                continue ;
            }

            touchTile(layer, childTile, frameState);

            if(childTile.contentState === ContentState.LOADED){
                processTile(layer, childTile, frameState);
                selectPagelods.push(pageLod);
                continue;
            }

            if(childTile.contentState === ContentState.READY){
                stack.push(childTile);
                continue ;
            }

            loadTile(layer, childTile, frameState);
            selectPagelods.push(pageLod);

        }
    }

    for(let i = 0,j = selectPagelods.length;i < j;i++){
        const pageLod = selectPagelods[i];
        layer._renderQueue = layer._renderQueue.concat(pageLod.renderEntities);
    }

    updatePriority(layer, frameState);
};

export default S3MLayerScheduler;