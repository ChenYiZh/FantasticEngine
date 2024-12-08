/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
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
using FantasyEngine.Collections;
using FantasyEngine.Log;
using Newtonsoft.Json;
using ProtoBuf;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace FantasyEngine.Network.Server.Entity
{
    /// <summary>
    /// 当属性发生变化时执行
    /// </summary>
    /// <param name="sender">被修改的对象</param>
    /// <param name="propertyName">属性名称</param>
    /// <param name="oldValue">原本的数据</param>
    /// <param name="value">现在的数据</param>
    public delegate void OnPropertyModified<T>(T sender, string propertyName, object oldValue, object value) where T : Entity;

    /// <summary>
    /// Model基类
    /// </summary>
    public abstract class Entity : IEntity
    {
        /// <summary>
        /// 当前实例的存储状态
        /// </summary>
        [JsonIgnore]
        public abstract EStorageState State { get; }

        /// <summary>
        /// 锁
        /// </summary>
        protected internal object SyncRoot = new object();

        /// <summary>
        /// 是否已经发生变化
        /// </summary>
        [JsonIgnore]
        public bool IsModified
        {
            get
            {
                //lock (SyncRoot)
                //{
                //    return ModifiedType != EModifyType.UnModified;
                //}
                object syncRoot = SyncRoot;
                bool lockTaken = false;
                try
                {
                    Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                    return ModifiedType != EModifyType.UnModified;
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
        /// 上次修改的时间
        /// </summary>
        [NonSerialized]
        private DateTime _modifiedTime = TimeLord.Now;

        /// <summary>
        /// 上次修改的时间
        /// Set: 只在初始化时起作用
        /// </summary>        
        [EntityField, JsonProperty, ProtoMember(ushort.MaxValue)]
        public DateTime ModifiedTime
        {
            get
            {
                //lock (SyncRoot)
                //{
                //    return modifiedTime;
                //}
                object syncRoot = SyncRoot;
                bool lockTaken = false;
                try
                {
                    Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                    return GetModifiedTime();
                }
                finally
                {
                    if (lockTaken)
                    {
                        Monitor.Exit(syncRoot);
                    }
                }
            }
            internal set
            {
                //lock (SyncRoot)
                //{
                //    modifiedTime = value;
                //}
                object syncRoot = SyncRoot;
                bool lockTaken = false;
                try
                {
                    Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                    _modifiedTime = value;
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
        /// 操作类型
        /// </summary>
        [NonSerialized]
        private EModifyType _modifiedType = EModifyType.Add;

        /// <summary>
        /// 操作类型
        /// </summary>
        [JsonIgnore]
        public EModifyType ModifiedType
        {
            get
            {
                //lock (SyncRoot)
                //{
                //    return modifiedType;
                //}
                object syncRoot = SyncRoot;
                bool lockTaken = false;
                try
                {
                    Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                    return _modifiedType;
                }
                finally
                {
                    if (lockTaken)
                    {
                        Monitor.Exit(syncRoot);
                    }
                }
            }
            //EntitySet调用
            internal set
            {
                //lock (SyncRoot)
                //{
                //    modifiedType = value;
                //}
                object syncRoot = SyncRoot;
                bool lockTaken = false;
                try
                {
                    Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                    _modifiedType = value;
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

        [JsonIgnore]
        private ICollection<PropertyEntity> Children { get; set; } = new HashSet<PropertyEntity>();

        /// <summary>
        /// 注入时调用
        /// </summary>
        protected void NotifyPropertyModified(string propertyName, object oldValue, object value)
        {
            OnNotifyPropertyModified(propertyName, oldValue, value);
        }

        /// <summary>
        /// 属性调用的实现函数,外部需要锁
        /// </summary>
        internal virtual void OnNotifyPropertyModified(string propertyName, object oldValue, object value)
        {
            NotifyModified(EModifyType.Modify, propertyName);
            //设置数据关联
            lock (SyncRoot)
            {
                PropertyEntity property = value as PropertyEntity;
                if (property != null && oldValue == null)
                {
                    property.SetParent(this, propertyName);
                    Children.Add(property);

                    foreach (PropertyEntity child in Children)
                    {
                        child.ModifiedTime = TimeLord.Now;
                    }
                    return;
                }
                property = oldValue as PropertyEntity;
                if (property != null && value == null)
                {
                    Children.Remove(property);
                    property.RemoveFromParent();
                }
            }
        }

        /// <summary>
        /// 通知实例已经发生变化
        /// </summary>
        internal virtual void NotifyModified(EModifyType modifiedType, string propertyName = null)
        {
            lock (SyncRoot)
            {
                NotifyModifiedType(modifiedType);
                if (State == EStorageState.Stored)
                {
                    _modifiedTime = TimeLord.Now;
                }
            }
        }

        /// <summary>
        /// 更新状态
        /// </summary>
        private void NotifyModifiedType(EModifyType modifiedType)
        {
            switch (this._modifiedType)
            {
                case EModifyType.UnModified: this._modifiedType = modifiedType; break;
                case EModifyType.Modify:
                    {
                        if (modifiedType == EModifyType.Remove || modifiedType == EModifyType.Add)
                        {
                            this._modifiedType = modifiedType;
                        }
                    }
                    break;
                case EModifyType.Add:
                    {
                        if (modifiedType == EModifyType.Remove)
                        {
                            this._modifiedType = EModifyType.UnModified;
                        }
                    }
                    break;
            }
        }

        /// <summary>
        /// 修改已经提交时执行
        /// </summary>
        internal virtual void OnModificationCommitted()
        {
            ResetModifiedType();
            lock (SyncRoot)
            {
                //modifiedTime = TimeLord.Now;
                foreach (PropertyEntity child in Children)
                {
                    child.OnModificationCommitted();
                }
            }
        }

        /// <summary>
        /// 数据数据库中拉取下来
        /// </summary>
        internal virtual void OnPulledFromDb()
        {
            lock (SyncRoot)
            {
                ResetModifiedType();
                foreach (PropertyEntity child in Children)
                {
                    child.OnPulledFromDb();
                }
            }
        }
        /// <summary>
        /// 重置修改的状态
        /// </summary>
        internal virtual void ResetModifiedType()
        {
            //lock (SyncRoot)
            //{
            //    modifiedType = EModifyType.UnModified;
            //}
            object syncRoot = SyncRoot;
            bool lockTaken = false;
            try
            {
                Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                _modifiedType = EModifyType.UnModified;
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
        /// 用于判断是否作为冷数据来卸载。默认使用内部的修改时间，如果需要用自用的修改时间，比如使用登录时间，覆盖这个函数。
        /// </summary>
        /// <returns></returns>
        protected virtual DateTime GetModifiedTime()
        {
            return _modifiedTime;
        }
    }
}
