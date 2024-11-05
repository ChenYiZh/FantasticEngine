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
using System.Diagnostics;
using System.Linq;
using UnityEditor;
using UnityEngine;
namespace FantasticEngine
{
    /// <summary>
    /// 编辑器工具
    /// </summary>
    public class EditorTools : Editor
    {
        //获取根目录路径
        [MenuItem("GameObject/UI/GetRootName", false, 100)]
        static void GetRoot()
        {
            GameObject obj = Selection.activeGameObject;
            string rootPath = GetRoot(obj);
            rootPath = rootPath.Replace("GameRoot/UIRoot/", "");
            int startIndex = rootPath.IndexOf('/');
            if (startIndex > 0 && startIndex < rootPath.Length)
            {
                rootPath = rootPath.Substring(startIndex + 1);
            }
            UnityEngine.Debug.Log(rootPath);
            UnityEngine.Debug.LogError(rootPath);
            GUIUtility.systemCopyBuffer = rootPath;
        }

        static string GetRoot(GameObject obj)
        {
            if (obj != null)
            {
                Transform transform = obj.transform;
                string root = transform.name;
                while (transform.parent != null)
                {
                    transform = transform.parent;
                    root = transform.name + "/" + root;
                }

                return root;
            }

            return "";
        }


        [MenuItem("GameObject/Remove Missing Scripts")]
        static void RemoveMissingScripts()
        {
            List<GameObject[]> selections = Selection.gameObjects.Select(g => g.GetComponentsInChildren<Transform>(true).Select(t => t.gameObject).ToArray()).ToList();
            foreach (GameObject[] gameObjects in selections)
            {
                foreach (GameObject gameObject in gameObjects)
                {
#if UNITY_2017_1_OR_NEWER
                    GameObjectUtility.RemoveMonoBehavioursWithMissingScript(gameObject);
#endif
                }
            }
        }

        //[MenuItem("Tools/Atlas/Change Format", priority = 50)]
        //public static void ChangeFormat()
        //{
        //    var guids = AssetDatabase.FindAssets("t:Texture");
        //    string path = null;
        //    try
        //    {
        //        for (int i = 0; i < guids.Length; i++)
        //        {
        //            path = AssetDatabase.GUIDToAssetPath(guids[i]);
        //            if (path.EndsWith(".ttf") || path.EndsWith(".renderTexture") || path.EndsWith(".asset"))
        //            {
        //                continue;
        //            }

        //            if (EditorUtility.DisplayCancelableProgressBar("修改图片格式", path, i * 1.0f / guids.Length))
        //            {
        //                break;
        //            }

        //            var importer = AssetImporter.GetAtPath(path) as TextureImporter;
        //            if (importer.textureType == TextureImporterType.SingleChannel)
        //            {
        //                Debug.LogError("SingleChannel: " + path);
        //                continue;
        //            }

        //            var settings = importer.GetPlatformTextureSettings("iOS");
        //            bool reimport = !settings.overridden || settings.format != TextureImporterFormat.ASTC_4x4;
        //            if (reimport)
        //            {
        //                settings.overridden = true;
        //                settings.format = TextureImporterFormat.ASTC_4x4;
        //                importer.SetPlatformTextureSettings(settings);
        //            }

        //            settings = importer.GetPlatformTextureSettings("Android");
        //            bool reimport2 = !settings.overridden || settings.format != TextureImporterFormat.ASTC_4x4;
        //            if (reimport2)
        //            {
        //                settings.overridden = true;
        //                settings.format = TextureImporterFormat.ASTC_4x4;
        //                importer.SetPlatformTextureSettings(settings);
        //            }

        //            if (reimport || reimport2)
        //            {
        //                AssetDatabase.ImportAsset(path);
        //            }
        //        }
        //    }
        //    catch (Exception e)
        //    {
        //        Debug.LogError(path);
        //        Debug.LogException(e);
        //    }

        //    EditorUtility.ClearProgressBar();
        //}

        [MenuItem("Tools/Open/PersistentDataPath", false, 50)]
        static void OpenPersistentDataPath()
        {
            Process.Start("explorer.exe", Application.persistentDataPath.Replace("/", "\\"));
        }
    }
}