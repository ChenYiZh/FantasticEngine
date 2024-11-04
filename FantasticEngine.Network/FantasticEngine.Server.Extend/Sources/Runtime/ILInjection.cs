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
using FantasticEngine;
using FantasticEngine.IO;
using FantasticEngine.Log;
using Mono.Cecil;
using Mono.Cecil.Cil;
using Mono.Cecil.Pdb;
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;

namespace FantasticEngine.Network.Server
{
    /// <summary>
    /// 代码注入器，依赖Mono
    /// </summary>
    public static class ILInjection
    {
        /// <summary>
        /// 注入监听Model发生变化的代码
        /// </summary>
        /// <returns></returns>
        public static bool InjectEntityChangeEvent()
        {
            if (string.IsNullOrEmpty(Settings.AssemblyName))
            {
                return false;
            }
            //判断dll是否存在
            string assemblyFullname = FEPath.GetFullPath(Settings.AssemblyName);
            if (!File.Exists(assemblyFullname))
            {
                throw new FileNotFoundException(string.Format("Assembly not found: {0}", assemblyFullname));
            }

            //获取pdb文件，判断是否Debug
            string pdbFullname = Path.ChangeExtension(assemblyFullname, "pdb");
            PdbReaderProvider readerProvider = null;
            PdbWriterProvider writerProvider = null;
            bool isDebug = false;
            if (File.Exists(pdbFullname))
            {
                isDebug = true;
                readerProvider = new PdbReaderProvider();
                writerProvider = new PdbWriterProvider();
            }

            //加载assembly的描述文件，使用debug在于注入代码后断点位置会发生变化
            AssemblyDefinition definition = AssemblyDefinition.ReadAssembly(assemblyFullname, new ReaderParameters()
            {
                SymbolReaderProvider = readerProvider,
                ReadSymbols = isDebug,
                InMemory = true
            });

            //添加引用的搜索目录，由于保存dll时会检索引用
            BaseAssemblyResolver resolver = definition.MainModule.AssemblyResolver as BaseAssemblyResolver;
            if (resolver != null)
            {
                resolver.AddSearchDirectory(Environment.CurrentDirectory);
            }

            //判断有没有脚本发生变化
            bool hasChanged = false;

            //搜寻基类
            string entityName = FEType<Entity.Entity>.Type.FullName;
            string refDll = Path.Combine(Environment.CurrentDirectory, "FantasticEngine.Network.Server.Common.dll");
            ModuleDefinition module = ModuleDefinition.ReadModule(refDll);
            TypeDefinition entityType = module.GetType(entityName);
            MethodDefinition notifyMethod = null;
            foreach (MethodDefinition method in entityType.Methods)
            {
                if (method.Name == "NotifyPropertyModified")
                {
                    notifyMethod = method;
                    break;
                }
            }
            FieldDefinition syncRootField = null;
            foreach (FieldDefinition field in entityType.Fields)
            {
                if (field.Name == "SyncRoot")
                {
                    syncRootField = field;
                    break;
                }
            }

            if (notifyMethod == null || syncRootField == null)
            {
                FEConsole.WriteErrorFormatWithCategory(Categories.FOOLISH_SERVER, "The class Entity was not found!");
                return false;
            }

            try
            {
                _methodTypes = new GlobalMethodTypes(definition.MainModule, syncRootField, notifyMethod);
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.FOOLISH_SERVER, e);
                return false;
            }

            //获取所有类型进行处理
            IEnumerable<TypeDefinition> types = definition.MainModule.GetTypes();
            foreach (TypeDefinition type in types)
            {
                if (ProcessEntityType(type, entityType))
                {
                    hasChanged = true;
                }
            }

            //保存注入后程序集
            if (hasChanged)
            {
                definition.Write(assemblyFullname, new WriterParameters()
                {
                    SymbolWriterProvider = writerProvider,
                    WriteSymbols = isDebug
                });
            }

            return hasChanged;
        }

        /// <summary>
        /// 向Model层注入代码
        /// </summary>
        private static bool ProcessEntityType(TypeDefinition type, TypeDefinition entityType)
        {
            bool hasChanged = false;
            if (type.IsChildOf(entityType.FullName))
            {
                foreach (PropertyDefinition property in type.Properties)
                {
                    if (property.ContainsAttribute("EntityFieldAttribute"))
                    {
                        if (InjectAopCode(type, property))
                        {
                            //FConsole.WriteWarnWithCategory(Categories.MODEL, "{0} modified.", type.FullName);
                            hasChanged = true;
                        }
                    }
                }
            }
            return hasChanged;
        }

        /// <summary>
        /// 注入修改监听代码
        /// </summary>
        private static bool InjectAopCode(TypeDefinition type, PropertyDefinition property)
        {
            MethodDefinition setMethod = property.SetMethod;
            MethodDefinition getMethod = property.GetMethod;
            if (setMethod == null || setMethod.Body.Instructions.Count > 5 || getMethod == null || getMethod.Body.Instructions.Count > 5)
            {
                return false;
            }

            //寻找私有变量
            string propertyName = property.Name;
            FieldDefinition field = null;
            if (type.HasFields)
            {
                foreach (FieldDefinition itsField in type.Fields)
                {
                    if (itsField.Name.StartsWith("<" + propertyName + ">") || string.Equals(itsField.Name, "_" + propertyName, StringComparison.OrdinalIgnoreCase))
                    {
                        field = itsField;
                        break;
                    }
                }
            }
            if (field == null)
            {
                return false;
            }

            //Set方法注入
            GenerateSetMethod(setMethod, field, propertyName);

            //Get方法注入
            GenerateGetMethod(getMethod, field, propertyName);

            return true;
        }

        /// <summary>
        /// 全局需要用的引用
        /// </summary>
        private struct GlobalMethodTypes
        {
            /// <summary>
            /// bool
            /// </summary>
            public TypeReference BoolType { get; private set; }

            /// <summary>
            /// object
            /// </summary>
            public TypeReference ObjectType { get; private set; }

            /// <summary>
            /// 同步锁的对象
            /// </summary>
            public FieldReference SyncRootField { get; private set; }

            /// <summary>
            /// 通知函数引用
            /// </summary>
            public MethodReference NotifyMethod { get; private set; }

            /// <summary>
            /// Monitor锁
            /// </summary>
            public MethodReference MonitorEnter { get; private set; }

            /// <summary>
            /// Monitor锁
            /// </summary>
            public MethodReference MonitorExit { get; private set; }

            /// <summary>
            /// Object.Equals(A,B)
            /// </summary>
            public MethodReference EqualsMethod { get; private set; }

            /// <summary>
            /// 锁的Timeout
            /// </summary>
            public int MillisecondsTimeout { get; private set; }

            /// <summary>
            /// 赋值
            /// </summary>
            public GlobalMethodTypes(ModuleDefinition module, FieldDefinition syncRootField, MethodDefinition notifyMethod)
            {
                MillisecondsTimeout = 100;

                BoolType = module.ImportReference(FEType<bool>.Type);
                ObjectType = module.ImportReference(FEType<object>.Type);
                SyncRootField = module.ImportReference(syncRootField);
                NotifyMethod = module.ImportReference(notifyMethod);


                MethodInfo monitorEnterMethodInfo = null;
                MethodInfo[] monitorMethods = typeof(System.Threading.Monitor).GetMethods();

                foreach (MethodInfo method in monitorMethods)
                {
                    if (method.Name == "TryEnter")
                    {
                        ParameterInfo[] paras = method.GetParameters();
                        if (paras.Length == 3 && paras[1].ParameterType == FEType<int>.Type)
                        {
                            monitorEnterMethodInfo = method;
                            break;
                        }
                    }
                }

                //foreach (MethodInfo method in monitorMethods)
                //{
                //    if (method.Name == "TryEnter" && method.GetParameters().Length == 2)
                //    {
                //        monitorEnterMethodInfo = method;
                //        break;
                //    }
                //}

                if (monitorEnterMethodInfo == null)
                {
                    throw new Exception("The method: System.Threading.Monitor.TryEnter(object obj, int millisecondsTimeout, ref bool lockTaken); was not found!");
                }
                MonitorEnter = module.ImportReference(monitorEnterMethodInfo);
                MonitorExit = module.ImportReference(typeof(System.Threading.Monitor).GetMethod("Exit", new Type[] { typeof(object) }));
                EqualsMethod = module.ImportReference(FEType<object>.Type.GetMethod("Equals", new Type[] { typeof(object), typeof(object) }));
            }
        }

        /// <summary>
        /// 全局引用
        /// </summary>
        private static GlobalMethodTypes _methodTypes;

        /// <summary>
        /// 注入Set函数
        /// </summary>
        private static void GenerateSetMethod(
            MethodDefinition setMethod,
            FieldDefinition privateField,
            string propertyName)
        {
            setMethod.Body.Variables.Clear();
            ILProcessor worker = setMethod.Body.GetILProcessor();
            TypeReference paramType = setMethod.Parameters[0].ParameterType;

            // .maxstack 4
            setMethod.Body.MaxStackSize = 4;

            // .locals init (object V_0,
            //          bool V_1,
            //          bool V_2,
            //          object V_3)
            setMethod.Body.InitLocals = true;
            VariableDefinition V_0 = new VariableDefinition(_methodTypes.BoolType);
            VariableDefinition V_1 = new VariableDefinition(_methodTypes.ObjectType);
            VariableDefinition V_2 = new VariableDefinition(_methodTypes.ObjectType);
            VariableDefinition V_3 = new VariableDefinition(_methodTypes.BoolType);
            VariableDefinition V_4 = new VariableDefinition(_methodTypes.BoolType);
            VariableDefinition V_5 = new VariableDefinition(_methodTypes.BoolType);
            setMethod.Body.Variables.Add(V_0);
            setMethod.Body.Variables.Add(V_1);
            setMethod.Body.Variables.Add(V_2);
            setMethod.Body.Variables.Add(V_3);
            setMethod.Body.Variables.Add(V_4);
            setMethod.Body.Variables.Add(V_5);

            //生成 IL代码
            Instruction IL_0000 = worker.Create(OpCodes.Nop);
            Instruction IL_0001 = worker.Create(OpCodes.Ldc_I4_1);
            Instruction IL_0002 = worker.Create(OpCodes.Stloc_0);
            Instruction IL_0003 = worker.Create(OpCodes.Ldnull);
            Instruction IL_0004 = worker.Create(OpCodes.Stloc_1);
            Instruction IL_0005 = worker.Create(OpCodes.Ldarg_0);
            Instruction IL_0006 = worker.Create(OpCodes.Ldfld, _methodTypes.SyncRootField);
            Instruction IL_000b = worker.Create(OpCodes.Stloc_2);
            Instruction IL_000c = worker.Create(OpCodes.Ldc_I4_0);
            Instruction IL_000d = worker.Create(OpCodes.Stloc_3);


            Instruction IL_000e_0 = worker.Create(OpCodes.Nop);
            Instruction IL_000e = worker.Create(OpCodes.Ldloc_2);
            Instruction IL_000e_1 = worker.Create(OpCodes.Ldc_I4, _methodTypes.MillisecondsTimeout);
            Instruction IL_000f = worker.Create(OpCodes.Ldloca_S, V_3);
            Instruction IL_0011 = worker.Create(OpCodes.Call, _methodTypes.MonitorEnter);

            //Instruction IL_0016 = worker.Create(OpCodes.Nop);
            Instruction IL_0017 = worker.Create(OpCodes.Nop);
            Instruction IL_0018 = worker.Create(OpCodes.Ldarg_0);
            Instruction IL_0019 = worker.Create(OpCodes.Ldfld, privateField);
            Instruction IL_001e = worker.Create(OpCodes.Box, paramType);
            Instruction IL_0023 = worker.Create(OpCodes.Ldarg_1);
            Instruction IL_0024 = worker.Create(OpCodes.Box, paramType);
            Instruction IL_0029 = worker.Create(OpCodes.Call, _methodTypes.EqualsMethod);

            Instruction IL_002e = worker.Create(OpCodes.Stloc_0);
            Instruction IL_002f = worker.Create(OpCodes.Ldloc_0);
            Instruction IL_0030 = worker.Create(OpCodes.Ldc_I4_0);
            Instruction IL_0031 = worker.Create(OpCodes.Ceq);
            Instruction IL_0033 = worker.Create(OpCodes.Stloc_S, V_4);
            Instruction IL_0035 = worker.Create(OpCodes.Ldloc_S, V_4);

            Instruction IL_004e = worker.Create(OpCodes.Nop);

            Instruction IL_0037 = worker.Create(OpCodes.Brfalse_S, IL_004e);
            Instruction IL_0039 = worker.Create(OpCodes.Nop);
            Instruction IL_003a = worker.Create(OpCodes.Ldarg_0);
            Instruction IL_003b = worker.Create(OpCodes.Ldfld, privateField);
            Instruction IL_0040 = worker.Create(OpCodes.Box, paramType);
            Instruction IL_0045 = worker.Create(OpCodes.Stloc_1);
            Instruction IL_0046 = worker.Create(OpCodes.Ldarg_0);
            Instruction IL_0047 = worker.Create(OpCodes.Ldarg_1);
            Instruction IL_0048 = worker.Create(OpCodes.Stfld, privateField);
            Instruction IL_004d = worker.Create(OpCodes.Nop);

            //IL_004e

            Instruction IL_005c = worker.Create(OpCodes.Ldloc_0);


            Instruction IL_004f = worker.Create(OpCodes.Leave_S, IL_005c);

            Instruction IL_0051 = worker.Create(OpCodes.Ldloc_3);

            Instruction IL_005b = worker.Create(OpCodes.Endfinally);

            Instruction IL_0052 = worker.Create(OpCodes.Brfalse_S, IL_005b);
            Instruction IL_0054 = worker.Create(OpCodes.Ldloc_2);
            Instruction IL_0055 = worker.Create(OpCodes.Call, _methodTypes.MonitorExit);
            Instruction IL_005a = worker.Create(OpCodes.Nop);

            //IL_005a
            //IL_005c

            Instruction IL_005d = worker.Create(OpCodes.Ldc_I4_0);
            Instruction IL_005e = worker.Create(OpCodes.Ceq);
            Instruction IL_0060 = worker.Create(OpCodes.Stloc_S, V_5);
            Instruction IL_0062 = worker.Create(OpCodes.Ldloc_S, V_5);

            Instruction IL_007b = worker.Create(OpCodes.Ret);

            Instruction IL_0064 = worker.Create(OpCodes.Brfalse_S, IL_007b);
            Instruction IL_0066 = worker.Create(OpCodes.Nop);
            Instruction IL_0067 = worker.Create(OpCodes.Ldarg_0);
            Instruction IL_0068 = worker.Create(OpCodes.Ldstr, propertyName);
            Instruction IL_006d = worker.Create(OpCodes.Ldloc_1);
            Instruction IL_006e = worker.Create(OpCodes.Ldarg_1);
            Instruction IL_006f = worker.Create(OpCodes.Box, paramType);
            Instruction IL_0074 = worker.Create(OpCodes.Call, _methodTypes.NotifyMethod);

            Instruction IL_0079 = worker.Create(OpCodes.Nop);
            Instruction IL_007a = worker.Create(OpCodes.Nop);

            //IL_007b


            //注入
            setMethod.Body.Instructions.Clear();
            setMethod.Body.Instructions.Add(IL_0000);
            setMethod.Body.Instructions.Add(IL_0001);
            setMethod.Body.Instructions.Add(IL_0002);
            setMethod.Body.Instructions.Add(IL_0003);
            setMethod.Body.Instructions.Add(IL_0004);
            setMethod.Body.Instructions.Add(IL_0005);
            setMethod.Body.Instructions.Add(IL_0006);
            setMethod.Body.Instructions.Add(IL_000b);
            setMethod.Body.Instructions.Add(IL_000c);
            setMethod.Body.Instructions.Add(IL_000d);
            setMethod.Body.Instructions.Add(IL_000e_0);
            setMethod.Body.Instructions.Add(IL_000e);
            setMethod.Body.Instructions.Add(IL_000e_1);
            setMethod.Body.Instructions.Add(IL_000f);
            setMethod.Body.Instructions.Add(IL_0011);
            //setMethod.Body.Instructions.Add(IL_0016);
            setMethod.Body.Instructions.Add(IL_0017);
            setMethod.Body.Instructions.Add(IL_0018);
            setMethod.Body.Instructions.Add(IL_0019);
            setMethod.Body.Instructions.Add(IL_001e);
            setMethod.Body.Instructions.Add(IL_0023);
            setMethod.Body.Instructions.Add(IL_0024);
            setMethod.Body.Instructions.Add(IL_0029);
            setMethod.Body.Instructions.Add(IL_002e);
            setMethod.Body.Instructions.Add(IL_002f);
            setMethod.Body.Instructions.Add(IL_0030);
            setMethod.Body.Instructions.Add(IL_0031);
            setMethod.Body.Instructions.Add(IL_0033);
            setMethod.Body.Instructions.Add(IL_0035);
            setMethod.Body.Instructions.Add(IL_0037);
            setMethod.Body.Instructions.Add(IL_0039);
            setMethod.Body.Instructions.Add(IL_003a);
            setMethod.Body.Instructions.Add(IL_003b);
            setMethod.Body.Instructions.Add(IL_0040);
            setMethod.Body.Instructions.Add(IL_0045);
            setMethod.Body.Instructions.Add(IL_0046);
            setMethod.Body.Instructions.Add(IL_0047);
            setMethod.Body.Instructions.Add(IL_0048);
            setMethod.Body.Instructions.Add(IL_004d);
            setMethod.Body.Instructions.Add(IL_004e);
            setMethod.Body.Instructions.Add(IL_004f);
            setMethod.Body.Instructions.Add(IL_0051);
            setMethod.Body.Instructions.Add(IL_0052);
            setMethod.Body.Instructions.Add(IL_0054);
            setMethod.Body.Instructions.Add(IL_0055);
            setMethod.Body.Instructions.Add(IL_005a);
            setMethod.Body.Instructions.Add(IL_005b);
            setMethod.Body.Instructions.Add(IL_005c);
            setMethod.Body.Instructions.Add(IL_005d);
            setMethod.Body.Instructions.Add(IL_005e);
            setMethod.Body.Instructions.Add(IL_0060);
            setMethod.Body.Instructions.Add(IL_0062);
            setMethod.Body.Instructions.Add(IL_0064);
            setMethod.Body.Instructions.Add(IL_0066);
            setMethod.Body.Instructions.Add(IL_0067);
            setMethod.Body.Instructions.Add(IL_0068);
            setMethod.Body.Instructions.Add(IL_006d);
            setMethod.Body.Instructions.Add(IL_006e);
            setMethod.Body.Instructions.Add(IL_006f);
            setMethod.Body.Instructions.Add(IL_0074);
            setMethod.Body.Instructions.Add(IL_0079);
            setMethod.Body.Instructions.Add(IL_007a);
            setMethod.Body.Instructions.Add(IL_007b);



            //try-catch注入
            ExceptionHandler exceptionHandler = new ExceptionHandler(ExceptionHandlerType.Finally);
            exceptionHandler.TryStart = IL_000e_0;
            exceptionHandler.TryEnd = IL_0051;
            exceptionHandler.HandlerStart = IL_0051;
            exceptionHandler.HandlerEnd = IL_005c;

            setMethod.Body.ExceptionHandlers.Clear();
            setMethod.Body.ExceptionHandlers.Add(exceptionHandler);
        }

        /// <summary>
        /// 注入Get函数
        /// </summary>
        private static void GenerateGetMethod(
            MethodDefinition getMethod,
            FieldDefinition privateField,
            string propertyName)
        {
            getMethod.Body.Variables.Clear();
            ILProcessor worker = getMethod.Body.GetILProcessor();

            // .maxstack 2
            getMethod.Body.MaxStackSize = 2;

            // .locals init (object V_0,
            //          bool V_1,
            //          retType V_2)
            getMethod.Body.InitLocals = true;
            VariableDefinition V_0 = new VariableDefinition(_methodTypes.ObjectType);
            VariableDefinition V_1 = new VariableDefinition(_methodTypes.BoolType);
            VariableDefinition V_2 = new VariableDefinition(getMethod.ReturnType);
            VariableDefinition V_3 = new VariableDefinition(_methodTypes.BoolType);
            getMethod.Body.Variables.Add(V_0);
            getMethod.Body.Variables.Add(V_1);
            getMethod.Body.Variables.Add(V_2);
            getMethod.Body.Variables.Add(V_3);

            //生成 IL代码
            Instruction IL_0000 = worker.Create(OpCodes.Nop);
            Instruction IL_0001 = worker.Create(OpCodes.Ldarg_0);
            Instruction IL_0002 = worker.Create(OpCodes.Ldfld, _methodTypes.SyncRootField);
            Instruction IL_0007 = worker.Create(OpCodes.Stloc_0);
            Instruction IL_0008 = worker.Create(OpCodes.Ldc_I4_0);
            Instruction IL_0009 = worker.Create(OpCodes.Stloc_1);



            Instruction IL_000a_0 = worker.Create(OpCodes.Nop);
            Instruction IL_000a = worker.Create(OpCodes.Ldloc_0);

            Instruction IL_000a_1 = worker.Create(OpCodes.Ldc_I4, _methodTypes.MillisecondsTimeout);

            //Instruction IL_000a_1 = worker.Create(OpCodes.Ldc_I4_4, 0x3e8);

            Instruction IL_000b = worker.Create(OpCodes.Ldloca_S, V_1);
            Instruction IL_000d = worker.Create(OpCodes.Call, _methodTypes.MonitorEnter);

            //Instruction IL_0012 = worker.Create(OpCodes.Nop);
            Instruction IL_0013 = worker.Create(OpCodes.Nop);
            Instruction IL_0014 = worker.Create(OpCodes.Ldarg_0);
            Instruction IL_0015 = worker.Create(OpCodes.Ldfld, privateField);
            Instruction IL_001a = worker.Create(OpCodes.Stloc_2);

            Instruction IL_0028 = worker.Create(OpCodes.Ldloc_2);

            Instruction IL_001b = worker.Create(OpCodes.Leave_S, IL_0028);


            Instruction IL_001d = worker.Create(OpCodes.Ldloc_1);


            Instruction IL_0027 = worker.Create(OpCodes.Endfinally);

            Instruction IL_001e = worker.Create(OpCodes.Brfalse_S, IL_0027);

            Instruction IL_0020 = worker.Create(OpCodes.Ldloc_0);
            Instruction IL_0021 = worker.Create(OpCodes.Call, _methodTypes.MonitorExit);
            Instruction IL_0026 = worker.Create(OpCodes.Nop);

            Instruction IL_0029 = worker.Create(OpCodes.Ret);


            //注入
            getMethod.Body.Instructions.Clear();
            getMethod.Body.Instructions.Add(IL_0000);
            getMethod.Body.Instructions.Add(IL_0001);
            getMethod.Body.Instructions.Add(IL_0002);
            getMethod.Body.Instructions.Add(IL_0007);
            getMethod.Body.Instructions.Add(IL_0008);
            getMethod.Body.Instructions.Add(IL_0009);
            getMethod.Body.Instructions.Add(IL_000a_0);
            getMethod.Body.Instructions.Add(IL_000a);
            getMethod.Body.Instructions.Add(IL_000a_1);
            getMethod.Body.Instructions.Add(IL_000b);
            getMethod.Body.Instructions.Add(IL_000d);
            //getMethod.Body.Instructions.Add(IL_0012);
            getMethod.Body.Instructions.Add(IL_0013);
            getMethod.Body.Instructions.Add(IL_0014);
            getMethod.Body.Instructions.Add(IL_0015);
            getMethod.Body.Instructions.Add(IL_001a);
            getMethod.Body.Instructions.Add(IL_001b);
            getMethod.Body.Instructions.Add(IL_001d);
            getMethod.Body.Instructions.Add(IL_001e);
            getMethod.Body.Instructions.Add(IL_0020);
            getMethod.Body.Instructions.Add(IL_0021);
            getMethod.Body.Instructions.Add(IL_0026);
            getMethod.Body.Instructions.Add(IL_0027);
            getMethod.Body.Instructions.Add(IL_0028);
            getMethod.Body.Instructions.Add(IL_0029);


            //try-catch注入
            ExceptionHandler exceptionHandler = new ExceptionHandler(ExceptionHandlerType.Finally);
            exceptionHandler.TryStart = IL_000a_0;
            exceptionHandler.TryEnd = IL_001d;
            exceptionHandler.HandlerStart = IL_001d;
            exceptionHandler.HandlerEnd = IL_0028;

            getMethod.Body.ExceptionHandlers.Clear();
            getMethod.Body.ExceptionHandlers.Add(exceptionHandler);
        }
    }
}
