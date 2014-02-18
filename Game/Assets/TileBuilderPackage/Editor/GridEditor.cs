using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;

[CustomEditor (typeof(Grid))]
public class GridEditor  : Editor
{

    private Grid grid;
    private Object TilePrefab;

    public void OnEnable()
    {
        grid = (Grid)target;
        
        // Remove first so it doesnt stack
        SceneView.onSceneGUIDelegate -= GridUpdate;
        // Then add delegate 
        SceneView.onSceneGUIDelegate += GridUpdate;
    }

    private void Awake()
    {     
        TilePrefab = Resources.Load(@"[Prefabs]/TilePrefab"); //The base prefab that is being used to build upon.
    }
    
    private void OnSceneGUI()
    {
        Event e = Event.current;
        
        if (e.type == EventType.mouseDrag && e.button == 0)
        { 
            grid.selectedGameObjects = Selection.gameObjects;
                
            foreach (GameObject go in grid.selectedGameObjects)
            {
                if (go.GetComponent<TileAligner>() == null)
                {
                    go.AddComponent<TileAligner>();
                    //Debug.Log("Added TileAligner Component to Selected GameObjects!");
                }
            }
        } else if (e.type == EventType.MouseUp)
        {
            if (grid.selectedGameObjects != null)
            {
                foreach (GameObject go in grid.selectedGameObjects)
                {
                    if (go.GetComponent<TileAligner>() != null)
                    {
                        DestroyImmediate(go.GetComponent<TileAligner>());
                        grid.selectedGameObjects = null;
                        //Debug.Log("Deleted TileAligner Component from Selected GameObjects!");
                    }
                }
            }
        }
    }
    
    private void GridUpdate(SceneView sceneview)
    {
        Event e = Event.current;
        Ray ray = Camera.current.ScreenPointToRay(new Vector3(e.mousePosition.x, -e.mousePosition.y + Camera.current.pixelHeight));
        Vector3 mousePosition = ray.origin;
        Vector3 aligned = new Vector3(Mathf.Floor(mousePosition.x / grid.gridWidth) * grid.gridWidth + grid.gridWidth / 2.0f, Mathf.Floor(mousePosition.y / grid.gridHeight) * grid.gridHeight + grid.gridHeight / 2.0f, 0.0f);
        
        //----------------------------- Pointing at a gridspace that is not yet filled -----------------------------//
        if (e.isKey && e.keyCode == grid.buildingKey)
        {
            if (!grid.editMode)
            {
                return;
            }
            
            GameObject obj;
            Object prefab = TilePrefab;
           
            if (!Physics2D.GetRayIntersection(ray))
            {
                if (prefab)
                {
                    if (grid.selectedTileInteger != 0)
                    {
                        if (!grid.stackableObject)
                        {
                            obj = (GameObject)PrefabUtility.InstantiatePrefab(prefab);
                    
                            obj.transform.position = aligned;
                        
                            InstantiatePrefabSettings(obj);
                            Undo.RegisterCreatedObjectUndo(obj, "Create: " + obj.name);
                        }
                    }
                }
                //----------------------------- END -----------------------------//
                
                
                
                //----------------------------- Pointing at a gridspace that is already filled -----------------------------//
                //NOTE: If you dont want to stack objects, simply delete this entire Else statement 
                
                //If you get into this coding block, that means that you are pointing at a Grid space that is already being used by another object.
            } else
            {
                //Check if the prefab you want to build with exists.
                if (prefab)
                {
                    //Check if the selectedTileInteger is bigger than 0. In this Editor, 0 means null, no block. Always keep an empty spriteObject for space you dont want to use (Like backgrounds).
                    if (grid.selectedTileInteger != 0)
                    {
                        //Check if the CURRENT selected tile object is able to stack.
                        if (grid.stackableObject)
                        {
                            //Check if the item we want to build upon is in fact a tile, and has a class attached to it called TileObjectRawData.
                            if (Physics2D.GetRayIntersection(ray).collider.gameObject.GetComponent<TileObjectRawData>() != null)
                            {
                                //Explanation: The object we want to stack with should be TRUE, the object we want to build upon should be FALSE.
                                //Check if the object that is being pointed at, its stackableObject bool is FALSE. If it is FALSE, that means that it is able to being build upon with a new TRUE stackable object. 
                                //The new TRUE object will return if you want to create another building block ontop of the newely created tile. 
                                //(This means that you are able to create up to two layers ontop of each other, for instance a Miscelanious item ontop of a Environmental Building Block).
                                if (!Physics2D.GetRayIntersection(ray).collider.gameObject.GetComponent<TileObjectRawData>().stackableObject)
                                {
                                    obj = (GameObject)PrefabUtility.InstantiatePrefab(prefab);
                        
                                    obj.transform.position = aligned;
                        
                                    //Store original layerdepth.
                                    int initialDepth = grid.layerDepth;
                                    //Apply +1 so that the object is moved ontop of the original object.
                                    grid.layerDepth = Physics2D.GetRayIntersection(ray).collider.gameObject.GetComponent<TileObjectRawData>().layerDepth + 1;
                                    InstantiatePrefabSettings(obj);
                            
                                    //Restore its orignial layerdepth.
                                    grid.layerDepth = initialDepth;
                                    Undo.RegisterCreatedObjectUndo(obj, "Create: " + obj.name);
                                }
                            }
                        }
                    }
                }
            }
            //----------------------------- END -----------------------------//
            
            
            //----------------------------- Deleting an object -----------------------------//
        } else if (e.isKey && e.keyCode == grid.deletingKey)
        {
            if (!grid.editMode)
            {
                return;
            }
            
            foreach (GameObject obj in Selection.gameObjects)
            {
                Undo.DestroyObjectImmediate(obj);
                DestroyImmediate(obj);
            }
            //----------------------------- END -----------------------------//
            
            
            //----------------------------- Copying an object -----------------------------//
        } else if (e.isKey && e.keyCode == grid.copyKey)
        {
            if (!grid.editMode)
            {
                return;
            }
            
            CopySelectedTile(Selection.activeGameObject);
        }
        //----------------------------- END -----------------------------//
    }

    private void InstantiatePrefabSettings(GameObject go)
    {
        RetrieveSpriteDataInteger(go);
        AttachCollisionToSprite(go);
        AttachLayerDepthToSprite(go);
        AttachSpriteObjectToParent(go);
        grid.AttachNameToSprite(go);
        SaveTileCreationData(go);
    }


    private void RetrieveSpriteDataInteger(GameObject go)
    {
        foreach (KeyValuePair<int, Sprite> pair in ImageLibrary.spriteDictionary)
        {
            if (pair.Key == grid.selectedTileInteger)
            {
                go.GetComponent<SpriteRenderer>().sprite = pair.Value;
            }
        }
    }
    
    private void AttachCollisionToSprite(GameObject go)
    {
        switch (grid.collisionIndex)
        {
            case 0://"Polygon":
                go.AddComponent<PolygonCollider2D>();
                break;
            case 1://"Box":
                go.AddComponent<BoxCollider2D>();
                break;
            case 2://"Cirle":
                go.AddComponent<CircleCollider2D>();
                break;
            case 3://"Edge":
                go.AddComponent<EdgeCollider2D>();
                break;    
            case 4://"None":
                break;
            default:
                break;
        }
    }
    
    private void AttachLayerDepthToSprite(GameObject go)
    {
        go.GetComponent<SpriteRenderer>().sortingOrder = grid.layerDepth;
    }
    
    private void AttachSpriteObjectToParent(GameObject go)
    {
        go.transform.parent = grid.transform;
    }
    
    private void SaveTileCreationData(GameObject go)
    {
        var c = go.AddComponent<TileObjectRawData>();
        
        c.collisionIndex = grid.collisionIndex;
        c.layerDepth = grid.layerDepth;
        c.selectedTileInteger = grid.selectedTileInteger;
        c.selectedTileName = grid.selectedTileName;
        c.stackableObject = grid.stackableObject;
    }
    
    private void CopySelectedTile(GameObject go)
    {
        if (go.GetComponent<TileObjectRawData>() != null)
        {
            var c = go.GetComponent<TileObjectRawData>();
        
            grid.collisionIndex = c.collisionIndex;
            grid.layerDepth = c.layerDepth;
            grid.selectedTileInteger = c.selectedTileInteger;
            grid.selectedTileName = c.selectedTileName;
            grid.stackableObject = c.stackableObject;
        }
    }
    
    public override void OnInspectorGUI()
    {  
        GUILayout.BeginHorizontal();
        GUILayout.Label(" Grid Width ");
        grid.gridWidth = EditorGUILayout.FloatField(grid.gridWidth, GUILayout.Width(50));
        GUILayout.EndHorizontal();
        
        GUILayout.BeginHorizontal();
        GUILayout.Label(" Grid Height ");
        grid.gridHeight = EditorGUILayout.FloatField(grid.gridHeight, GUILayout.Width(50));
        GUILayout.EndHorizontal();
        
        GUILayout.BeginHorizontal();
        GUILayout.Label(" Grid Color ");
        grid.color = EditorGUILayout.ColorField(grid.color, GUILayout.Width(170));
        GUILayout.EndHorizontal();

        GUILayout.Space(20);
        
        if (GUILayout.Button(" Open Editor Window ", GUILayout.Width(255)))
        {
            GridWindow window = (GridWindow)EditorWindow.GetWindow(typeof(GridWindow));
            window.minSize = new Vector2(720f, 400f);
        }

        SceneView.RepaintAll();
    }
    
    private void OnDestroy()
    {
        SceneView.onSceneGUIDelegate -= GridUpdate;
    }
}