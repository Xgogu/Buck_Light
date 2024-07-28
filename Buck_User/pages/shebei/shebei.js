// pages/shebei/shebei.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    peidui:0,
    connect:0,
    disconnect:0,
    goodslist: [
      {
        id: 1,
        name: '台灯',
        status: '商品状态1',
        imageUrl: '/images/light.png', // 图片路径，请替换成实际图片路径
      },
      {
        id: 2,
        name: '台灯',
        status: '商品状态2',
        imageUrl: '/utils/img/lighton.png', // 图片路径，请替换成实际图片路径
      },
    ],
  },
  initgoodlist()
  {
    wx.getStorage({
      key: 'connectedDeviceId',
      success: function(res) {
        connectedDeviceId = res.data;
        console.log('已存储的用户头像：', connectedDeviceId);},
        fail: function(err) {
          console.error('读取存储失败', err);
        }
      })

  },
  /**
   * 生命周期函数--监听页面加载
   */
  onLoad(options) {

  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady() {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow() {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide() {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload() {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh() {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom() {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage() {

  }
})