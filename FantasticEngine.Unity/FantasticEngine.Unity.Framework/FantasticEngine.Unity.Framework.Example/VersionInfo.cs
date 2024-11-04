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
using System.Collections;
using System.Collections.Generic;
#if UNITY_EDITOR
using UnityEditor;
#endif
using UnityEngine;
using UnityEngine.Serialization;
namespace FantasticEngine
{
    // public enum EBuildType
    // {
    //     /// <summary>
    //     /// 开发版
    //     /// </summary>
    //     develop,
    //
    //     /// <summary>
    //     /// 测试版
    //     /// </summary>
    //     test,
    //
    //     /// <summary>
    //     /// 发布版
    //     /// </summary>
    //     release
    // }

    [CreateAssetMenu]
    public sealed class VersionInfo : ScriptableObject
    {
        [SerializeField] private EPlatform _platform = EPlatform.Desktop;

        public EPlatform Platform
        {
            get { return _platform; }
        }

        // [SerializeField] private EBuildType _type = EBuildType.develop;
        //
        // public EBuildType Type
        // {
        //     get { return _type; }
        // }

        [SerializeField] private int _majorVersion = 1;

        public int MajorVersion
        {
            get { return _majorVersion; }
        }

        [SerializeField] private int _minorVersion = 0;

        public int MinorVersion
        {
            get { return _minorVersion; }
        }

        [SerializeField] public int modVersion = 0;

        [SerializeField] public string builtTime;

        // public int ModVersion
        // {
        //     get { return modVersion; }
        // }

        public static string GetVersion()
        {
            return GameRoot.Root.Version.ToString();
        }

        public string GetBuiltPlatform()
        {
            return Platform.ToString();
        }

        public static string GetRuntimePlatform()
        {
            return Application.platform.ToString();
        }
#if UNITY_EDITOR
    public bool CheckBuildTarget(BuildTarget target)
    {
        switch (target)
        {
            case BuildTarget.WebGL:
                _platform = EPlatform.WebGL;
                return true;
            case BuildTarget.Android:
                _platform = EPlatform.WebGL;
#if PICO
                _platform = EPlatform.Pico;
#endif
                return true;
            case BuildTarget.StandaloneWindows:
            case BuildTarget.StandaloneWindows64:
                _platform = EPlatform.Desktop;
                return true;
            default:
                return false;
        }
    }
#endif
        public override string ToString()
        {
            return
                $"{MajorVersion}.{MinorVersion}.{modVersion}.{builtTime}" +
                //$"-{_type.ToString().Substring(0, 1).ToLower()}" +
                $" ({GetBuiltPlatform()})";
        }
    }
}