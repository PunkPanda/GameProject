using System;
using System.Linq;
using System.Reflection;
using UnityEditor;
using UnityEngine;
using System.Collections;
using Object = UnityEngine.Object;

public class InspectorLockToggle {
    private static EditorWindow _mouseOverWindow;
    private static InspectorLockToggle _instance;

    private InspectorLockToggle() {
    }
    
    public static InspectorLockToggle Instance
    {
        get
        {
            if (_instance == null) {
                _instance = new InspectorLockToggle();    
            }
            
            return _instance;
        }
    }
    
    [MenuItem("Editor/Toggle Inspector Lock &q")]
    public static void ToggleInspectorLock() {
        if (_mouseOverWindow == null) {
            if (!EditorPrefs.HasKey("LockableInspectorIndex")) {
                EditorPrefs.SetInt("LockableInspectorIndex", 0);
            }
            int i = EditorPrefs.GetInt("LockableInspectorIndex");
            
            Type type = Assembly.GetAssembly(typeof(Editor)).GetType("UnityEditor.InspectorWindow");
            Object[] findObjectsOfTypeAll = Resources.FindObjectsOfTypeAll(type);
            _mouseOverWindow = (EditorWindow)findObjectsOfTypeAll [i];
        }
        
        if (_mouseOverWindow != null && _mouseOverWindow.GetType().Name == "InspectorWindow") {
            Type type = Assembly.GetAssembly(typeof(Editor)).GetType("UnityEditor.InspectorWindow");
            PropertyInfo propertyInfo = type.GetProperty("isLocked");
            bool value = (bool)propertyInfo.GetValue(_mouseOverWindow, null);
            propertyInfo.SetValue(_mouseOverWindow, !value, null);
            _mouseOverWindow.Repaint();
        }
    }
    
    [MenuItem("Editor/Clear Console Log #&c")]
    public static void ClearConsole() {
        Type type = Assembly.GetAssembly(typeof(Editor)).GetType("UnityEditorInternal.LogEntries");
        type.GetMethod("Clear").Invoke(null, null);
    }
}