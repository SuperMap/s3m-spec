/**
 * S3M图层填充风格.
 *
 * @exports FillStyle
 *
 * @see Style3D
 */
const FillStyle = {
    /**
     * 填充模式
     *
     * @type {Number}
     * @constant
     */
    Fill : 0,

    /**
     * 线框模式
     *
     * @type {Number}
     * @constant
     */
    WireFrame : 1,

    /**
     * 填充和线框模式
     *
     * @type {Number}
     * @constant
     */
    Fill_And_WireFrame : 2
};

export default  Object.freeze(FillStyle);