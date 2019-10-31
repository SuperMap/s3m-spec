define([
    './Enum/ContentState'
],function(
    ContentState
    ){
    "use strict";
    function S3MLayerScheduler (){
        this._stack = [];
    }

    function selectRootTiles(layer, stack, frameState) {
        stack.length = 0;
        for(let i = 0,j = layer._rootTiles.length;i < j;i++){
            let rootTile = layer._rootTiles[i];
            rootTile.updateVisibility(frameState);
            if(!rootTile.visible) {
                continue ;
            }

            stack.push(rootTile);
        }
    }

    function sortComparator(a, b) {
        return a.distanceToCamera - b.distanceToCamera;
    }


    function updateChildren(layer, tile, stack, frameState) {
        let i;
        let children = tile.children;
        let length = children.length;

        for (i = 0; i < length; ++i) {
            children[i].updateVisibility(frameState);
        }

        children.sort(sortComparator);
        for (i = 0; i < length; ++i) {
            let child = children[i];
            if (child.visible) {
                stack.push(child);
            }
        }
    }

    function selectTile(layer, tile, frameState) {
        if(tile.renderable) {
            layer._selectedTiles.push(tile);
            tile.selectedFrame = frameState.frameNumber;
        }
    }

    function loadTile(layer, tile, frameState) {
        if(tile.requestedFrame === frameState.frameNumber || tile.contentState !== ContentState.UNLOADED || tile.isLeafTile) {
            return ;
        }

        layer._requestTiles.push(tile);
        tile.requestedFrame = frameState.frameNumber;
    }

    function touchTile(layer, tile, frameState) {
        if (tile.touchedFrame === frameState.frameNumber) {
            return;
        }

        layer._cache.touch(tile);
        tile.touchedFrame = frameState.frameNumber;
    }

    function traversal(layer, stack, frameState) {
        while(stack.length) {
            let tile = stack.pop();
            if(!tile.visible) {
                continue;
            }

            touchTile(layer, tile, frameState);

            if (tile.canTraverse()) {
                updateChildren(layer, tile, stack, frameState);
                continue ;
            }

            selectTile(layer, tile, frameState);
            loadTile(layer, tile, frameState);
        }
    }

    S3MLayerScheduler.prototype.scheduler = function(layer, frameState) {
        layer._requestTiles.length = 0;
        layer._selectedTiles.length = 0;
        let stack = this._stack;
        selectRootTiles(layer, stack, frameState);
        traversal(layer, stack, frameState);
    };

    return S3MLayerScheduler;
});
