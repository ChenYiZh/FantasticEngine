/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
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
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using FantasticEngine.IO;
using FantasticEngine.Log;
using FantasticEngine.Network.IO;

namespace FantasticEngine.Network
{
    /// <summary>
    /// 消息基类
    /// </summary>
    public abstract class GameAction
    {
        /// <summary>
        /// 消息Id
        /// </summary>
        public long MsgId { get; private set; }

        /// <summary>
        /// ActionId
        /// </summary>
        public int ActionId { get; private set; }

        /// <summary>
        /// 是否统计时间
        /// </summary>
        public bool AnalysisTime { get; protected set; } = true;

        /// <summary>
        /// 警告超时时间毫秒，0为全部统计
        /// </summary>
        public int AlertTimeout { get; protected set; } = 1000;

        ///// <summary>
        ///// 计时器
        ///// </summary>
        //private Stopwatch watch = null;

        ///// <summary>
        ///// 是否在计时
        ///// </summary>
        //private bool IsTiming { get { return watch != null && watch.IsRunning; } }

        /// <summary>
        /// Action执行的时机
        /// </summary>
        private DateTime EnterTime { get; set; }

        /// <summary>
        /// 接收到的消息
        /// </summary>
        private IMessageReader Reader { get; set; }

        /// <summary>
        /// 刚创建时处理，所有参数都还没有赋值
        /// </summary>
        public virtual void Awake() { }

        /// <summary>
        /// 类名
        /// </summary>
        private string _typeName = null;

        /// <summary>
        /// 类名
        /// </summary>
        protected string TypeName
        {
            get
            {
                if (_typeName == null)
                {
                    _typeName = GetType().Name;
                }
                return _typeName;
            }
        }

        /// <summary>
        /// 工作函数
        /// </summary>
        /// <param name="actionId"></param>
        /// <param name="reader"></param>
        internal void Work(int actionId, IMessageReader reader)
        {
            MsgId = reader.MsgId;
            ActionId = actionId;
            Reader = reader;
            AnalysisTime = true;
            AlertTimeout = 1000;
            try
            {
                Awake();
                //if (AnalysisTime)
                //{
                //    if (watch == null) { watch = new Stopwatch(); }
                //    watch.Restart();
                //}
                EnterTime = DateTime.Now;
                SetReader(reader);
                if (Check())
                {
                    TakeAction(reader);
                }
                if (AnalysisTime)
                {
                    //watch.Stop();
                    //TimeSpan deltaTime = watch.Elapsed;
                    TimeSpan deltaTime = DateTime.Now - EnterTime;
                    if (deltaTime.TotalMilliseconds > AlertTimeout)
                    {
                        FEConsole.WriteWarnFormatWithCategory(Categories.ACTION, "{0} is timeout, {1}ms.", TypeName, (int)deltaTime.TotalMilliseconds);
                    }
                }
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.ACTION, "An error occurred on process action", e);
            }
            //if (watch != null)
            //{
            //    watch.Reset();
            //}
        }

        /// <summary>
        /// 判断有效性
        /// </summary>
        public virtual bool Check()
        {
            return true;
        }

        /// <summary>
        /// 消息预处理
        /// </summary>
        public virtual void SetReader(IMessageReader reader) { }
        /// <summary>
        /// 消息处理
        /// </summary>
        public abstract void TakeAction(IMessageReader reader);
    }
}
