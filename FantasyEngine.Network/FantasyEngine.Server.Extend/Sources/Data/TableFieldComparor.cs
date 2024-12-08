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
using FantasyEngine.Network.Server.Entity;
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasyEngine.Network.Server
{
    /// <summary>
    /// 表的列表判断是否更改
    /// </summary>
    public struct TableFieldComparor
    {
        /// <summary>
        /// 需要执行的操作
        /// </summary>
        public enum EOperation
        {
            /// <summary>
            /// 不需要操作
            /// </summary>
            UnModified = 0,
            /// <summary>
            /// 只需要修改字段属性
            /// </summary>
            Modified = 1,
            /// <summary>
            /// 先删除后添加
            /// </summary>
            DeleteThenInsert = 2,
            /// <summary>
            /// 添加新列
            /// </summary>
            ToInsert = 10,
            /// <summary>
            /// 删除列
            /// </summary>
            Deleted = 20,
        }
        /// <summary>
        /// 列名
        /// </summary>
        public string FieldName
        {
            get
            {
                if (TableField != null)
                {
                    return TableField.Name;
                }
                if (DbFieldInfo != null)
                {
                    return DbFieldInfo.Name;
                }
                throw new NullReferenceException("TableFieldComparor can not compare two null values.");
            }
        }
        /// <summary>
        /// 需要做的操作
        /// </summary>
        public EOperation Operation { get; private set; }
        /// <summary>
        /// 缓存中的结构
        /// </summary>
        public IEntityField TableField { get; private set; }
        /// <summary>
        /// 数据库中的结构
        /// </summary>
        public IEntityField DbFieldInfo { get; private set; }
        /// <summary>
        /// 是不是只有主键判断进行了修改
        /// </summary>
        public bool OnlyIsKeyChanged { get; private set; }
        /// <summary>
        /// 是不是只有索引进行了修改
        /// </summary>
        public bool OnlyIsIndexChanged { get; private set; }
        /// <summary>
        /// 判断是否成功
        /// </summary>
        public bool IsError { get; private set; }
        /// <summary>
        /// 判断两个结构是否需要进行操作
        /// </summary>
        /// <param name="tableField">缓存中的结构</param>
        /// <param name="dbFieldInfo">数据库中的结构</param>
        public TableFieldComparor(IEntityField tableField, IEntityField dbFieldInfo)
        {
            Operation = EOperation.UnModified;
            TableField = tableField;
            DbFieldInfo = dbFieldInfo;
            IsError = false;
            OnlyIsKeyChanged = false;
            OnlyIsIndexChanged = false;
            bool onlyIsIndexChanged, onlyIsKeyChanged;
            EOperation operation;
            IsError = !Compare(tableField, dbFieldInfo, out operation, out onlyIsKeyChanged, out onlyIsIndexChanged);
            Operation = operation;
            OnlyIsKeyChanged = onlyIsKeyChanged;
            OnlyIsIndexChanged = onlyIsIndexChanged;
        }
        /// <summary>
        /// 判断两个结构是否需要进行操作
        /// </summary>
        public static bool Compare(IEntityField tableField, IEntityField dbFieldInfo, out EOperation operation, out bool onlyIsKeyChanged, out bool onlyIsIndexChanged)
        {
            onlyIsKeyChanged = onlyIsIndexChanged = false;
            operation = EOperation.UnModified;
            if (tableField == null && dbFieldInfo == null)
            {
                throw new NullReferenceException("TableFieldComparor can not compare two null values.");
            }
            if (tableField == null)
            {
                operation = EOperation.Deleted;
                return true;
            }
            if (dbFieldInfo == null)
            {
                operation = EOperation.ToInsert;
                return true;
            }
            onlyIsKeyChanged = tableField.IsKey != dbFieldInfo.IsKey;
            onlyIsIndexChanged = tableField.IsIndex != dbFieldInfo.IsIndex;
            if (tableField.IsIndex != dbFieldInfo.IsIndex)
            {
                onlyIsKeyChanged = false;
                operation = EOperation.Modified;
            }
            if (tableField.IsKey != dbFieldInfo.IsKey)
            {
                onlyIsIndexChanged = false;
                operation = EOperation.Modified;
            }
            if (tableField.DefaultValue.GetString() != dbFieldInfo.DefaultValue.GetString()
                || tableField.Nullable != dbFieldInfo.Nullable)
            {
                onlyIsIndexChanged = false;
                operation = EOperation.Modified;
            }
            if (tableField.FieldType != dbFieldInfo.FieldType)
            {
                onlyIsIndexChanged = false;
                switch (tableField.FieldType)
                {
                    case ETableFieldType.Blob:
                    case ETableFieldType.LongBlob:
                        {
                            operation = EOperation.Modified;
                            if (dbFieldInfo.FieldType != ETableFieldType.Blob
                                && dbFieldInfo.FieldType != ETableFieldType.LongBlob)
                            {
                                return false;
                            }
                        }
                        break;

                    case ETableFieldType.Byte:
                    case ETableFieldType.UShort:
                    case ETableFieldType.UInt:
                    case ETableFieldType.ULong:
                    case ETableFieldType.SByte:
                    case ETableFieldType.Short:
                    case ETableFieldType.Int:
                    case ETableFieldType.Long:
                        {
                            operation = EOperation.Modified;
                            if (dbFieldInfo.FieldType != ETableFieldType.Byte
                                && dbFieldInfo.FieldType != ETableFieldType.UShort
                                && dbFieldInfo.FieldType != ETableFieldType.UInt
                                && dbFieldInfo.FieldType != ETableFieldType.ULong
                                && dbFieldInfo.FieldType != ETableFieldType.SByte
                                && dbFieldInfo.FieldType != ETableFieldType.Short
                                && dbFieldInfo.FieldType != ETableFieldType.Int
                                && dbFieldInfo.FieldType != ETableFieldType.Long)
                            {
                                return false;
                            }
                        }
                        break;

                    case ETableFieldType.Float:
                    case ETableFieldType.Double:
                        {
                            operation = EOperation.Modified;
                            if (dbFieldInfo.FieldType != ETableFieldType.Float
                                && dbFieldInfo.FieldType != ETableFieldType.Double)
                            {
                                return false;
                            }
                        }
                        break;
                    case ETableFieldType.String:
                    case ETableFieldType.Text:
                    case ETableFieldType.LongText:
                        {
                            operation = EOperation.Modified;
                            if (dbFieldInfo.FieldType != ETableFieldType.String
                                && dbFieldInfo.FieldType != ETableFieldType.Text
                                && dbFieldInfo.FieldType != ETableFieldType.LongText)
                            {
                                return false;
                            }
                        }
                        break;
                    case ETableFieldType.Auto:
                    case ETableFieldType.Bit:
                    case ETableFieldType.DateTime:
                    case ETableFieldType.Error:
                    default: return false;
                }
            }
            return true;
        }
    }
}
