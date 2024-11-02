/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2024 ChenYiZh
https://space.bilibili.com/9308172

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/
using System.Collections.Generic;
using System.Text;
using UnityEngine;

namespace FantasyEngine
{
    /// <summary>
    /// 配置管理
    /// </summary>
    [CreateAssetMenu(menuName = "Configs/GameDefines")]
    public sealed partial class GameDefines : ScriptableObject
    {
        /// <summary>
        /// 获取单例
        /// </summary>
        public static GameDefines Instance
        {
            get { return GameRoot.Root.GameDefines; }
        }
        /// <summary>
        /// 是否输出错误日志
        /// </summary>
        [SerializeField] private bool _bLogError = true;

        public bool LogError
        {
            get { return _bLogError; }
        }

        /// <summary>
        /// 是否输出所有错误日志
        /// </summary>
        [SerializeField] private bool _bLogAll = true;

        public bool LogAll
        {
            get { return _bLogAll; }
        }

        /// <summary>
        /// 是否只使用本地表
        /// </summary>
        public bool UseLocalTable = false;

        /// <summary>
        /// 读表路径
        /// </summary>
        [SerializeField] private string _resourceTablePath = "Tables/";

        public string ResourceTablePath
        {
            get { return _resourceTablePath; }
        }

        /// <summary>
        /// 可读写目录
        /// </summary>
        // [SerializeField] private string writableTablePath = Application.persistentDataPath;

        public string WritableTablePath
        {
            get { return Application.persistentDataPath; }
        }

        /// <summary>
        /// 在线Url
        /// </summary>
        public string OnlineBaseUrl;

        /// <summary>
        /// 在线校验表文件Url
        /// </summary>
        /// <returns></returns>
        public string GetOnlineIdxUrl()
        {
            string url = $"{OnlineBaseUrl}/assets.idx";
            return FEUtility.CheckUrl(url);
        }

        /// <summary>
        /// 在线表Url
        /// </summary>
        /// <returns></returns>
        public string GetOnlineTablesUrl()
        {
            string url = $"{OnlineBaseUrl}{ResourceTablePath}";
            return FEUtility.CheckUrl(url);
        }

        ///// <summary>
        ///// 图集列表
        ///// </summary>
        //[SerializeField] private List<SpriteAtlas> atlasDefines;

        /// <summary>
        /// panel动画曲线
        /// </summary>
        [SerializeField] private AnimationCurve _panelRoundAnimCurve;

        public AnimationCurve PanelRoundAnimCurve
        {
            get { return _panelRoundAnimCurve; }
        }

        /// <summary>
        /// panel动画曲线
        /// </summary>
        [SerializeField] private AnimationCurve _panelOpacityAnimCurve;

        public AnimationCurve PanelOpacityAnimCurve
        {
            get { return _panelOpacityAnimCurve; }
        }

        /// <summary>
        /// panel动画曲线
        /// </summary>
        [SerializeField] private AnimationCurve _panelScaleAnimCurve;

        public AnimationCurve PanelScaleAnimCurve
        {
            get { return _panelScaleAnimCurve; }
        }

        /// <summary>
        /// 音乐音量大小
        /// </summary>
        public float _musicVolume = 1.0f;

        /// <summary>
        /// 音效音量
        /// </summary>
        public float _audioVolume = 1.0f;

        /// <summary>
        /// 文本编码
        /// </summary>
        public readonly Encoding UTF8Encoding = new UTF8Encoding(false);

        /// <summary>
        /// Panel的Resource路径
        /// </summary>
        [SerializeField] private string _resourceUIPanelPath = "UIPanels/";

        public string ResourceUIPanelPath
        {
            get { return _resourceUIPanelPath; }
        }

        /// <summary>
        /// Tips的Resource路径
        /// </summary>
        [SerializeField] private string _resourceUITipsPath = "UITips/";

        public string ResourceUITipsPath
        {
            get { return _resourceUITipsPath; }
        }

        /// <summary>
        /// 音效目录
        /// </summary>
        [SerializeField] private string _resourceAudioPath = "Audio/";

        public string ResourceAudioPath
        {
            get { return _resourceAudioPath; }
        }

        /// <summary>
        /// BGM目录
        /// </summary>
        [SerializeField] private string _resourceMusicPath = "Music/";

        public string ResourceMusicPath
        {
            get { return _resourceMusicPath; }
        }

        /// <summary>
        /// 时间格式化
        /// </summary>
        [SerializeField] private string _timeFormat = "HH:mm:ss";

        public string TimeFormat
        {
            get { return _timeFormat; }
        }

        /// <summary>
        /// 日期格式化
        /// </summary>
        [SerializeField] private string _dateFormat = "yyyy-MM-dd";

        public string DateFormat
        {
            get { return _dateFormat; }
        }

        public float uiWidth;

        public float uiHeight;

        public float uiAbsWidth;

        public float uiAbsHeight;

        /// <summary>
        /// 完整时间格式化
        /// </summary>
        [SerializeField] private string _dateTimeFormat = "yyyy-MM-dd HH:mm:ss";

        public string DateTimeFormat
        {
            get { return _dateTimeFormat; }
        }

        //public Sprite GetSprite(string name)
        //{
        //    foreach (var atlas in atlasDefines)
        //    {
        //        var sprite = atlas.GetSprite(name);
        //        if (sprite != null)
        //        {
        //            return sprite;
        //        }
        //    }

        //    return null;
        //}

        public void Initialize() { }
    }
}