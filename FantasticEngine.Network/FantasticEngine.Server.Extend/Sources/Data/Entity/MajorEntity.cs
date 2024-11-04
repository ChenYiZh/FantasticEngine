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
using FantasticEngine.Log;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Web;

namespace FantasticEngine.Network.Server.Entity
{
    /// <summary>
    /// 当前实例的存储状态
    /// </summary>
    public enum EStorageState
    {
        /// <summary>
        /// 刚创建的
        /// </summary>
        New,
        /// <summary>
        /// 已经在堆栈中
        /// </summary>
        Stored,
        /// <summary>
        /// 移除了
        /// </summary>
        Removed,
    }
    /// <summary>
    /// 基于表的类区别于Minor
    /// </summary>
    public abstract class MajorEntity : Entity
    {
        /// <summary>
        /// 当前实例的存储状态
        /// </summary>
        [JsonIgnore]
        private EStorageState _state = EStorageState.New;
        /// <summary>
        /// 当前实例的存储状态
        /// </summary>
        [JsonIgnore]
        public override EStorageState State
        {
            get
            {
                //lock (SyncRoot)
                //{
                //    return state;
                //}
                object syncRoot = SyncRoot;
                bool lockTaken = false;
                try
                {
                    Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                    return _state;
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
        /// 当前实例的存储状态
        /// </summary>
        /// <param name="state"></param>
        internal void SetState(EStorageState state)
        {
            //lock (SyncRoot)
            //{
            //    this.state = state;
            //}
            object syncRoot = SyncRoot;
            bool lockTaken = false;
            try
            {
                Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                this._state = state;
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
        /// 表名
        /// </summary>
        [JsonIgnore]
        internal ITableScheme TableScheme { get; private set; }
        /// <summary>
        /// EntityKey被修改前的数据
        /// </summary>
        [JsonIgnore]
        private EntityKey _oldEntityKey;
        /// <summary>
        /// Redis主键名称
        /// </summary>
        [JsonIgnore]
        private EntityKey _entityKey;
        /// <summary>
        /// 判断数据是否长时间没有修改
        /// </summary>
        [JsonIgnore]
        internal bool IsExpired { get { return (TimeLord.Now - ModifiedTime) > Settings.ColdEntitiesCheckoutInterval; } }

        /// <summary>
        /// 构造函数
        /// </summary>
        public MajorEntity()
        {
            Type type = GetType();
            TableScheme = DataContext.GetTableScheme(type);
            IReadOnlyList<ITableFieldScheme> keyFields = ((TableScheme)TableScheme).KeyFields;
            object[] keys = new object[keyFields.Count];
            for (int i = 0; i < keys.Length; i++)
            {
                keys[i] = keyFields[i].DefaultValue;
            }
            _oldEntityKey = _entityKey = new EntityKey(type, keys);
        }

        /// <summary>
        /// 当属性发生变化时执行
        /// </summary>
        public event OnPropertyModified<MajorEntity> OnPropertyModified;

        /// <summary>
        /// 属性调用的实现函数
        /// </summary>
        internal override void OnNotifyPropertyModified(string propertyName, object oldValue, object value)
        {
            if (TableScheme != null && TableScheme.FieldsByProperty.ContainsKey(propertyName) && TableScheme.FieldsByProperty[propertyName].IsKey)
            {
                UpdateEntityKey(TableScheme.FieldsByProperty[propertyName], value);
            }
            base.OnNotifyPropertyModified(propertyName, oldValue, value);
            try
            {
                OnPropertyModified?.Invoke(this, propertyName, oldValue, value);
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.MODEL, e);
            }
        }
        /// <summary>
        /// 更新存入Redis的Key
        /// </summary>
        /// <param name="tableField"></param>
        /// <param name="value"></param>
        private void UpdateEntityKey(ITableFieldScheme tableField, object value)
        {
            TableScheme tableScheme = TableScheme as TableScheme;
            if (tableScheme == null) return;
            int index = -1;
            for (int i = 0; i < tableScheme.KeyFields.Count; i++)
            {
                if (tableScheme.KeyFields[i] == tableField)
                {
                    index = i;
                    break;
                }
            }
            if (index == -1)
            {
                FEConsole.WriteErrorFormatWithCategory(tableScheme.Type.Name, "Failed to generate entity id... check this class");
                return;
            }
            _entityKey.keys[index] = value;
            _entityKey.RefreshKeyName();
        }

        /// <summary>
        /// 数据中心提交更新
        /// </summary>
        internal override void NotifyModified(EModifyType modifiedType, string propertyName = null)
        {
            base.NotifyModified(modifiedType, propertyName);
            if (State == EStorageState.Stored)
            {
                DataContext.EntityPool[TableScheme.Type].OnModified(GetEntityKey(), this);
            }
        }

        /// <summary>
        /// 主键
        /// </summary>
        /// <returns></returns>
        public EntityKey GetEntityKey()
        {
            //lock (SyncRoot)
            //{
            //    return entityKey;
            //}
            object syncRoot = SyncRoot;
            bool lockTaken = false;
            try
            {
                Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                return _entityKey;
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
        /// EntityKey被修改前的数据
        /// </summary>
        /// <returns></returns>
        internal EntityKey GetOldEntityKey()
        {
            //lock (SyncRoot)
            //{
            //    return oldEntityKey;
            //}
            object syncRoot = SyncRoot;
            bool lockTaken = false;
            try
            {
                Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                return _oldEntityKey;
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
        /// 判断主键是否发生了变化
        /// </summary>
        internal bool KeyIsModified()
        {
            //lock (SyncRoot)
            //{
            //    return !string.IsNullOrEmpty(oldEntityKey) && oldEntityKey != entityKey;
            //}
            object syncRoot = SyncRoot;
            bool lockTaken = false;
            try
            {
                Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                return !string.IsNullOrEmpty(_oldEntityKey) && _oldEntityKey != _entityKey;
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
        /// 重置时，把Key也重置
        /// </summary>
        internal override void ResetModifiedType()
        {
            base.ResetModifiedType();
            RefreshEntityKey();
        }

        /// <summary>
        /// 刷新Key
        /// </summary>
        internal void RefreshEntityKey()
        {
            //lock (SyncRoot)
            //{
            //    oldEntityKey = entityKey;
            //}
            object syncRoot = SyncRoot;
            bool lockTaken = false;
            try
            {
                Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                _oldEntityKey = _entityKey;
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
        /// 数据数据库中拉取下来
        /// </summary>
        internal override void OnPulledFromDb()
        {
            base.OnPulledFromDb();
            //lock (SyncRoot)
            //{
            //    state = EStorageState.Stored;
            //}
            object syncRoot = SyncRoot;
            bool lockTaken = false;
            try
            {
                Monitor.TryEnter(syncRoot, Settings.LockerTimeout, ref lockTaken);
                _state = EStorageState.Stored;
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
}
