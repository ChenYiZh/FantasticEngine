/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2022-2030 ChenYiZh
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
using System.Text;
using System.Threading;

namespace FantasticEngine.Network.Server.Entity
{
    /// <summary>
    /// 用于属性的结构类
    /// </summary>
    public abstract class PropertyEntity : Entity
    {
        /// <summary>
        /// 父实例
        /// </summary>
        [NonSerialized]
        private Entity _parent = null;

        /// <summary>
        /// 父实例
        /// </summary>
        internal Entity Parent { get { lock (SyncRoot) { return _parent; } } }

        /// <summary>
        /// 当前实例的存储状态
        /// </summary>
        private EStorageState _state = EStorageState.New;

        /// <summary>
        /// 当前实例的存储状态
        /// </summary>
        public override EStorageState State
        {
            get
            {
                //lock (SyncRoot)
                //{
                //    return parent == null ? state : parent.State;
                //}
                object syncRoot = SyncRoot;
                bool lockTaken = false;
                try
                {
                    Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                    return _parent == null ? _state : _parent.State;

                }
                finally
                {
                    if (lockTaken)
                    {
                        Monitor.Exit(syncRoot);
                    }
                }
            }
        }

        /// <summary>
        /// 当前实例在父实例中的属性名称
        /// </summary>
        [NonSerialized]
        private string _propertyNameInParent = null;

        /// <summary>
        /// 当前实例在父实例中的属性名称
        /// </summary>
        internal string PropertyNameInParent
        {
            get
            {
                //lock (SyncRoot)
                //{
                //    return propertyNameInParent;
                //}
                object syncRoot = SyncRoot;
                bool lockTaken = false;
                try
                {
                    Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                    return _propertyNameInParent;

                }
                finally
                {
                    if (lockTaken)
                    {
                        Monitor.Exit(syncRoot);
                    }
                }
            }
        }

        /// <summary>
        /// 绑入父物体
        /// </summary>
        /// <param name="parent"></param>
        /// <param name="propertyName"></param>
        internal virtual void SetParent(Entity parent, string propertyName)
        {
            //lock (SyncRoot)
            //{
            //    this.parent = parent;
            //    this.propertyNameInParent = propertyName;
            //}
            object syncRoot = SyncRoot;
            bool lockTaken = false;
            try
            {
                Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                this._parent = parent;
                this._propertyNameInParent = propertyName;
            }
            finally
            {
                if (lockTaken)
                {
                    Monitor.Exit(syncRoot);
                }
            }
        }

        /// <summary>
        /// 从父物体中移除
        /// </summary>
        internal void RemoveFromParent()
        {
            //lock (SyncRoot)
            //{
            //    parent = null;
            //    propertyNameInParent = null;
            //    state = EStorageState.Removed;
            //}
            object syncRoot = SyncRoot;
            bool lockTaken = false;
            try
            {
                Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                _parent = null;
                _propertyNameInParent = null;
                _state = EStorageState.Removed;
            }
            finally
            {
                if (lockTaken)
                {
                    Monitor.Exit(syncRoot);
                }
            }
        }
        /// <summary>
        /// 通知修改
        /// </summary>
        /// <param name="modifiedType"></param>
        /// <param name="propertyName"></param>
        internal override void NotifyModified(EModifyType modifiedType, string propertyName = null)
        {
            base.NotifyModified(modifiedType, propertyName);
            lock (SyncRoot)
            {
                if (_parent != null && modifiedType != EModifyType.UnModified)
                {
                    _parent.NotifyModified(EModifyType.Modify, _propertyNameInParent);
                }
            }
        }
    }
}
