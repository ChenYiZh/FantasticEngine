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
using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace FantasyEngine
{
    public static class XmlExtensions
    {
        public static string GetValue(this XmlNode node, string def)
        {
            if (node == null) { return def; }
            return string.IsNullOrEmpty(node.InnerText) ? def : node.InnerText;
        }

        public static bool GetValue(this XmlNode node, bool def)
        {
            if (node == null) { return def; }
            bool result;
            return bool.TryParse(node.InnerText, out result) ? result : def;
        }

        public static short GetValue(this XmlNode node, short def)
        {
            if (node == null) { return def; }
            short result;
            return short.TryParse(node.InnerText, out result) ? result : def;
        }

        public static int GetValue(this XmlNode node, int def)
        {
            if (node == null) { return def; }
            int result;
            return int.TryParse(node.InnerText, out result) ? result : def;
        }

        public static long GetValue(this XmlNode node, long def)
        {
            if (node == null) { return def; }
            long result;
            return long.TryParse(node.InnerText, out result) ? result : def;
        }

        public static ushort GetValue(this XmlNode node, ushort def)
        {
            if (node == null) { return def; }
            ushort result;
            return ushort.TryParse(node.InnerText, out result) ? result : def;
        }

        public static uint GetValue(this XmlNode node, uint def)
        {
            if (node == null) { return def; }
            uint result;
            return uint.TryParse(node.InnerText, out result) ? result : def;
        }

        public static ulong GetValue(this XmlNode node, ulong def)
        {
            if (node == null) { return def; }
            ulong result;
            return ulong.TryParse(node.InnerText, out result) ? result : def;
        }

        public static string GetValue(this XmlNode node, string attribute, string def)
        {
            if (node == null) { return def; }
            return node.Attributes.GetNamedItem(attribute).GetValue(def);
        }

        public static bool GetValue(this XmlNode node, string attribute, bool def)
        {
            if (node == null) { return def; }
            return node.Attributes.GetNamedItem(attribute).GetValue(def);
        }

        public static short GetValue(this XmlNode node, string attribute, short def)
        {
            if (node == null) { return def; }
            return node.Attributes.GetNamedItem(attribute).GetValue(def);
        }

        public static int GetValue(this XmlNode node, string attribute, int def)
        {
            if (node == null) { return def; }
            return node.Attributes.GetNamedItem(attribute).GetValue(def);
        }

        public static long GetValue(this XmlNode node, string attribute, long def)
        {
            if (node == null) { return def; }
            return node.Attributes.GetNamedItem(attribute).GetValue(def);
        }

        public static ushort GetValue(this XmlNode node, string attribute, ushort def)
        {
            if (node == null) { return def; }
            return node.Attributes.GetNamedItem(attribute).GetValue(def);
        }

        public static uint GetValue(this XmlNode node, string attribute, uint def)
        {
            if (node == null) { return def; }
            return node.Attributes.GetNamedItem(attribute).GetValue(def);
        }

        public static ulong GetValue(this XmlNode node, string attribute, ulong def)
        {
            if (node == null) { return def; }
            return node.Attributes.GetNamedItem(attribute).GetValue(def);
        }

        public static string GetValue(this XmlAttribute attribute, string def)
        {
            if (attribute == null) { return def; }
            return string.IsNullOrEmpty(attribute.Value) ? def : attribute.Value;
        }

        public static bool GetValue(this XmlAttribute attribute, bool def)
        {
            if (attribute == null) { return def; }
            bool result;
            return bool.TryParse(attribute.Value, out result) ? result : def;
        }

        public static short GetValue(this XmlAttribute attribute, short def)
        {
            if (attribute == null) { return def; }
            short result;
            return short.TryParse(attribute.Value, out result) ? result : def;
        }

        public static int GetValue(this XmlAttribute attribute, int def)
        {
            if (attribute == null) { return def; }
            int result;
            return int.TryParse(attribute.Value, out result) ? result : def;
        }

        public static long GetValue(this XmlAttribute attribute, long def)
        {
            if (attribute == null) { return def; }
            long result;
            return long.TryParse(attribute.Value, out result) ? result : def;
        }

        public static ushort GetValue(this XmlAttribute attribute, ushort def)
        {
            if (attribute == null) { return def; }
            ushort result;
            return ushort.TryParse(attribute.Value, out result) ? result : def;
        }

        public static uint GetValue(this XmlAttribute attribute, uint def)
        {
            if (attribute == null) { return def; }
            uint result;
            return uint.TryParse(attribute.Value, out result) ? result : def;
        }

        public static ulong GetValue(this XmlAttribute attribute, ulong def)
        {
            if (attribute == null) { return def; }
            ulong result;
            return ulong.TryParse(attribute.Value, out result) ? result : def;
        }
    }
}
