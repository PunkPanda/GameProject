using UnityEngine;
using System.Collections;
using System.Collections.Generic;

[ExecuteInEditMode]
public class Grid : MonoBehaviour
{

    //Grid 1 by 1 units.
    public float gridWidth = 1.0f;
    public float gridHeight = 1.0f;
    public Color color = Color.white;
    
    public bool editMode = false;
    public bool stackableObject = false;
    public int collisionIndex = 0;
    public int layerDepth = 0;
    public int selectedTileInteger = 0;
    public string selectedTileName = "";
    public string selectedWindowTab;
    public string[] collisionModels = new string[]
    {
        "Polygon",
        "Box",
        "Circle",
        "Edge",
        "None"
    };
    
    //Capital letters require you to hold Shift. 'A' -> Shift + a.
    public KeyCode buildingKey = KeyCode.A;
    public KeyCode deletingKey = KeyCode.D;
    public KeyCode copyKey = KeyCode.C;
    
    public List<string> ListOfAddedCategoryStrings = new List<string>();
    public List<bool> ListOfAddedCategoryBools = new List<bool>();
    
    public List<List<string>> ListOfAllSubStringLists = new List<List<string>>();
    public List<List<bool>> ListOfAllSubBoolLists = new List<List<bool>>();
    
    public GameObject[] selectedGameObjects;
    
    private void OnDrawGizmos()
    {
        Vector3 pos = Camera.current.transform.position;
        Gizmos.color = this.color;
                  
        for (float x = pos.x - 1200.0f; x < pos.x + 1200.0f; x+= gridWidth)
        {
            Gizmos.DrawLine(new Vector3(Mathf.Floor(x / gridWidth) * gridWidth, -50.0f, 0.0f), new Vector3(Mathf.Floor(x / gridWidth) * gridWidth, 50.0f, 0.0f));
        }
        
        for (float y = pos.y - 800.0f; y < pos.y + 800.0f; y+= gridHeight)
        {
            Gizmos.DrawLine(new Vector3(-90.0f, Mathf.Floor(y / gridHeight) * gridHeight, 0.0f), new Vector3(90.0f, Mathf.Floor(y / gridHeight) * gridHeight, 0.0f));
        }
    }
    
    public void AttachNameToSprite(GameObject go)
    {
        go.name = ("[Tile] " + "[" + selectedTileName + "] [" + go.transform.position.x + "X] " + "[" + go.transform.position.y + "Y] " + "[" + layerDepth + "L]").ToString();
    }
}