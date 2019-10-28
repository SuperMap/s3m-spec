define([
    ], function(
        ) {
    'use strict';

    function S3MLayerCache() {
        this._list = new Cesium.DoublyLinkedList();
        this._sentinel = this._list.add();
        this._trimTiles = false;
    }

    S3MLayerCache.prototype.reset = function() {
        this._list.splice(this._list.tail, this._sentinel);
    };

    S3MLayerCache.prototype.touch = function(tile) {
        let node = tile.cacheNode;
        if (Cesium.defined(node)) {
            this._list.splice(this._sentinel, node);
        }
    };

    S3MLayerCache.prototype.add = function(tile) {
        if (!Cesium.defined(tile.cacheNode)) {
            tile.cacheNode = this._list.add(tile);
        }
    };

    S3MLayerCache.prototype.unloadTile = function(tileset, tile, unloadCallback) {
        let node = tile.cacheNode;
        if (!Cesium.defined(node)) {
            return;
        }

        this._list.remove(node);
        tile.cacheNode = undefined;
        unloadCallback(tileset, tile);
    };

    S3MLayerCache.prototype.unloadTiles = function(tileset, unloadCallback) {
        let trimTiles = this._trimTiles;
        this._trimTiles = false;
        let list = this._list;
        let maximumMemoryUsageInBytes = tileset.maximumMemoryUsage * 1024 * 1024;
        let sentinel = this._sentinel;
        let node = list.head;
        while ((node !== sentinel) && ((tileset.totalMemoryUsageInBytes > maximumMemoryUsageInBytes) || trimTiles)) {
            let tile = node.item;
            node = node.next;
            this.unloadTile(tileset, tile, unloadCallback);
        }
    };

    S3MLayerCache.prototype.trim = function() {
        this._trimTiles = true;
    };

    return S3MLayerCache;
});
