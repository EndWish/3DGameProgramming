using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.IO;
using System.Text;
using UnityEditor;


public class MakeFile : MonoBehaviour
{
    void WriteColor(Color c)
    {
        binaryWriter.Write(c.r);
        binaryWriter.Write(c.g);
        binaryWriter.Write(c.b);
        binaryWriter.Write(c.a);
    }

    void SaveMaterial(Material material)
    {
        BinaryWriter binaryWriter = new BinaryWriter(File.Open(string.Copy(material.name).Replace(" ", "_") + ".bin", FileMode.Create));
        binaryWriter.Write(material.GetColor())
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
