using UnityEngine;
using UnityEditor;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Text.RegularExpressions;

public class GridWindow : EditorWindow
{

    private Grid grid;
    
    private bool editModeButton = false;

    private List<Sprite> temporaryKeywordSpriteList = new List<Sprite>();
    private List<Texture> drawTextureInWindowList = new List<Texture>();
    private List<string> rowAndTagList = new List<string>();
    
    private string rowString = "";
    private string tagString = "";
    private int amountOfAddedRows;
    private int maximumAllowedRows = 10; //I highly suggest you dont use more than 10 rows because of internal use of the code flow.
    
    private int tileWidthAndOffset;
    
    private int maxTilesPerRowEditorWindow;
    private Vector2 scrollPos;
    
    private string editorDataLocation;
    
    public void Awake()
    {
        editorDataLocation = AssetDatabase.GetAssetPath(Resources.Load("SaveData/TagData"));
    
        grid = FindObjectOfType(typeof(Grid)) as Grid;
        ImageLibrary.GetSpriteData();
        ImageLibrary.GetTextureData();
    }
    
    //Each time you recompile this script, it will refresh the input tags and rows lists. This is being used for error prevention.
    void OnEnable()
    {
        DeleteAll();
        LoadEditorWindowSettings();
        
        //Whenever the window is popped, lock the inspector on the Grid gameObject.
        InspectorLockToggle.ToggleInspectorLock();
    }

    private void OnGUI()
    { 
        int buttonHeight = 18;
        
        GUILayout.BeginVertical();
        
        GUILayout.BeginHorizontal();
        amountOfAddedRows = grid.ListOfAddedCategoryStrings.Count;
        if (GUILayout.Button("Add Row" + " (" + amountOfAddedRows.ToString() + "/" + maximumAllowedRows.ToString() + ")", EditorStyles.miniButtonLeft, GUILayout.Width(position.width * 0.14f), GUILayout.Height(buttonHeight)))
        {
            if (grid.ListOfAddedCategoryStrings.Count >= maximumAllowedRows)
            { //Cant add more rows if cap of 10 has been reached.
                return;
            }

            if (rowString != "")
            {
                bool rowBool = false;
                
                grid.ListOfAddedCategoryBools.Add(rowBool);
                grid.ListOfAddedCategoryStrings.Add(rowString);
                rowString = "";
                
                List<string> addedSubStringList = new List<string>();
                List<bool> addedSubBoolList = new List<bool>();
                grid.ListOfAllSubStringLists.Add(addedSubStringList);
                grid.ListOfAllSubBoolLists.Add(addedSubBoolList);
            }
        }
        rowString = GUILayout.TextField(rowString, EditorStyles.textField, GUILayout.Width(position.width * 0.74f), GUILayout.Height(buttonHeight)); 
        if (rowString.Length >= 1 && char.IsDigit(rowString [0]))
        {
            rowString = Regex.Replace(rowString, @"[^a-zA-Z]", string.Empty);   //First character of the string cannot be a number.
        }   
        
        if (GUILayout.Button("Delete All", EditorStyles.miniButtonRight, GUILayout.Width(position.width * 0.10f), GUILayout.Height(buttonHeight)))
        {
            if (EditorUtility.DisplayDialog("Delete all?", "Are you sure you want to delete all rows and tags? ", "Delete", "Cancel"))
            {
                DeleteAll();
            }
        }                                                                    
        GUILayout.EndHorizontal();
        for (int i = 0; i < grid.ListOfAddedCategoryBools.Count; i++)
        {
            if (grid.ListOfAddedCategoryBools [i] = GUILayout.Toggle(grid.ListOfAddedCategoryBools [i], grid.ListOfAddedCategoryStrings [i], EditorStyles.miniButtonLeft, (grid.ListOfAddedCategoryBools [i] ? GUILayout.Width(position.width * 0.885f) : GUILayout.Width(position.width)), GUILayout.Height(buttonHeight)))
            {
                grid.ListOfAddedCategoryBools [i] = SetCategoryOption();
                
                GUILayout.BeginHorizontal();
                if (grid.ListOfAddedCategoryBools [i])
                {
                    if (GUILayout.Button("Add Tag", EditorStyles.miniButtonLeft, GUILayout.Width(position.width * 0.12f), GUILayout.Height(buttonHeight)))
                    {
                        if (tagString != "")
                        { 
                            bool tagBool = false;
                            
                            grid.ListOfAllSubBoolLists [i].Add(tagBool);
                            grid.ListOfAllSubStringLists [i].Add(tagString);
                            tagString = "";
                        }
                    }
                }
                tagString = GUILayout.TextField(tagString, EditorStyles.textField, GUILayout.Width(position.width * 0.15f), GUILayout.Height(buttonHeight));
                if (tagString.Length >= 1 && char.IsDigit(tagString [0]))
                {
                    tagString = Regex.Replace(rowString, @"[^a-zA-Z]", string.Empty);   //First character of the string cannot be a number.
                }
                
                for (int j = grid.ListOfAllSubBoolLists[i].Count-1; j >= 0; j--)
                {
                    if (grid.ListOfAllSubBoolLists [i] [j] = GUILayout.Toggle(grid.ListOfAllSubBoolLists [i] [j], grid.ListOfAllSubStringLists [i] [j], EditorStyles.miniButtonMid, GUILayout.Height(buttonHeight)))
                    {
                        grid.ListOfAllSubBoolLists [i] [j] = SetSubCategoryOption(grid.ListOfAllSubBoolLists [i]);
                        
                        //Make sure that once a tab has been pressed, it only extracts the keyword once. Dont execute this line every update frame because the keyword cant change if not pressed manually on another tab.
                        if (grid.selectedWindowTab != grid.ListOfAllSubStringLists [i] [j].ToString())
                        {
                            ExtractKeywordSpritesFromLibrary(grid.ListOfAllSubStringLists [i] [j].ToString());
                            grid.selectedWindowTab = grid.ListOfAllSubStringLists [i] [j].ToString();
                        }
                        
                        if (GUILayout.Button("X", EditorStyles.miniButtonRight, GUILayout.Height(buttonHeight), GUILayout.Width(position.width * 0.05f)))
                        {
                            if (EditorUtility.DisplayDialog("Delete Tag?", "Are you sure you want to delete this tag? ", "Delete", "Cancel"))
                            {
                                grid.ListOfAllSubStringLists [i].RemoveAt(j);
                                grid.ListOfAllSubBoolLists [i].RemoveAt(j);
                                
                                temporaryKeywordSpriteList.Clear();
                                drawTextureInWindowList.Clear();
                            }
                        }
                    }
                }
                
                GUILayout.EndHorizontal();  
                
                if (GUI.Button(new Rect(position.width * 0.889f, 20.5f + (buttonHeight * i + 2 * i), position.width * 0.10f, buttonHeight), "Delete Row", EditorStyles.miniButtonRight))
                {
                    
                    if (EditorUtility.DisplayDialog("Delete Row?", "Are you sure you want to delete this row? ", "Delete", "Cancel"))
                    {
                        grid.ListOfAddedCategoryBools.RemoveAt(i);
                        grid.ListOfAddedCategoryStrings.RemoveAt(i);
                        
                        grid.ListOfAllSubStringLists.RemoveAt(i);
                        grid.ListOfAllSubBoolLists.RemoveAt(i);
                    } 
                }
            }
        }

        GUILayout.Space(20);
        
        Rect TileArea = new Rect(200, 60 + 20 * grid.ListOfAddedCategoryBools.Count, position.width - 210, (position.height - 110) - (20 * grid.ListOfAddedCategoryBools.Count));   
        maxTilesPerRowEditorWindow = Mathf.FloorToInt(TileArea.width / (tileWidthAndOffset));
        
        GUILayout.BeginHorizontal();
        
        GUILayout.BeginArea(new Rect(10, 60 + 20 * grid.ListOfAddedCategoryBools.Count, 180, TileArea.height), EditorStyles.textField);
        GUILayout.Space(5);
        
        if (GUILayout.Button("Editor Mode is: " + (editModeButton ? " ON!" : " OFF!"), EditorStyles.miniButton))
        {
            editModeButton = !editModeButton;
            grid.editMode = !grid.editMode;
        }
        
        GUILayout.Space(10);
        GUILayout.Label(new GUIContent("How to use (Hover me)", "1. Read the read-me file. \n\n2. Make sure to set the Editor Mode button to On. \n\n3. Spawning a tile: Hover your mouse on a grid space and press " + grid.buildingKey + " to spawn your selected tile object. \n\n4. Deleting a tile: Select a tile object in the Scene View and press " + grid.deletingKey + ". \n\n5. Copying a tile: Select a tile object in the Scene View and press " + grid.copyKey + " to copy its parameters. \n\n6. Hover window elements to find out what they do. \n\n7. Start building epic levels! Enjoy!"), EditorStyles.boldLabel);
        GUILayout.Space(5);
        
        GUILayout.Label("Tile summary", EditorStyles.boldLabel);
        GUILayout.Label(new GUIContent("Amount of Tiles: " + temporaryKeywordSpriteList.Count.ToString(), "The amount of tiles currently in this category."));
        GUILayout.Label(new GUIContent("Tile Integer: " + grid.selectedTileInteger.ToString(), "The integer representing the selected tile. This integer is used in the code to find the correct spritesheet file."));
        GUILayout.Label(new GUIContent("Tile name: " + grid.selectedTileName.ToString(), "The name of the currentl selected tile."));
        grid.layerDepth = EditorGUILayout.IntField(new GUIContent("Layer Depth: ", "The depth the sprite will be placed at. Default depth is 0."), grid.layerDepth, EditorStyles.numberField);
        grid.stackableObject = EditorGUILayout.Toggle(new GUIContent("Stack this Object?", "Check this button if you want to place your currently selected tile ontop of another tile. This is useful if you would like to build a miscellaneous item ontop of an environmental building block. (Limited to two tiles stacked up)"), grid.stackableObject, EditorStyles.toggle);
        GUILayout.Space(5);
        GUILayout.Label(new GUIContent("Select collision model:", "Select the Collision Model you would like to have your sprite use."));
        grid.collisionIndex = EditorGUILayout.Popup(grid.collisionIndex, grid.collisionModels);
        GUILayout.EndArea();
        
        GUILayout.BeginArea(TileArea, EditorStyles.textField);
        scrollPos = GUI.BeginScrollView(new Rect(0, 0, TileArea.width, TileArea.height), scrollPos, new Rect(0, 0, 400, (Mathf.Ceil(temporaryKeywordSpriteList.Count / maxTilesPerRowEditorWindow) + 1) * (tileWidthAndOffset)));
        DrawTheExtractedSpritesInTheEditorWindow();
        GUI.EndScrollView();
        GUILayout.EndArea();
        GUILayout.EndHorizontal();
        GUILayout.EndVertical();
        
        CloseWindowOnEnteringPlayMode();
    }
    
    private bool SetCategoryOption()
    {
        for (int i = 0; i < grid.ListOfAddedCategoryBools.Count; i++)
        {
            grid.ListOfAddedCategoryBools [i] = false;
        }
        return true;
    }
    
    private bool SetSubCategoryOption(List<bool> _optionArray)
    {
        for (int i = 0; i < _optionArray.Count; i++)
        {
            _optionArray [i] = false;
        }     
        return true;
    }
    
    private void ExtractKeywordSpritesFromLibrary(string keyword)
    {
        temporaryKeywordSpriteList.Clear();
        
        foreach (Sprite sprite in ImageLibrary.spriteImages)
        {
            if (sprite.name.ToLower().Contains(keyword.ToLower()))
            {
                temporaryKeywordSpriteList.Add(sprite);
            }
        }
        
        PutExtractedKeywordsInDrawingList();
    }
    
    //Extract the Keywords once, put them in a seperate list, so that the same keywords arent being added every single update call.
    private void PutExtractedKeywordsInDrawingList()
    {
        drawTextureInWindowList.Clear();
        
        for (int i = 0; i < temporaryKeywordSpriteList.Count; i++)
        {
            for (int j = 0; j < ImageLibrary.textureImages.Count; j++)
            {
                if (temporaryKeywordSpriteList [i].name.ToLower() == ImageLibrary.textureImages [j].name.ToLower())
                {
                    drawTextureInWindowList.Add(ImageLibrary.textureImages [j]);
                }
            }
        }
    }
    
    //Draw the extracted sprite textures into the window.
    private void DrawTheExtractedSpritesInTheEditorWindow()
    {
        for (int i = 0; i < drawTextureInWindowList.Count; i++)
        {
        
            int xPos_StartPos = 10; //10 pixels from the left handside of the BeginArea() border.
            int yPos_StartPos = 10; //10 pixels from the top of the BeginArea() border.
            int xPos_Offset = 80;   //80 pixels total from left to right for the tileWidth (70) and offset between tiles (10).  
            int yPos_Offset = 80;   //You could also use: (int yPos_Offset = drawTextureInWindowList [i].height + 10) if you are sure that all your tiles are exactly the same size. This will scale the window nicely.
            int row = (int)(Mathf.Floor(i / maxTilesPerRowEditorWindow) * yPos_Offset); 
            int collumn = (int)(maxTilesPerRowEditorWindow * row);
                 
            tileWidthAndOffset = xPos_Offset + 1;
                          
            if (GUI.Button(new Rect(xPos_StartPos + xPos_Offset * i - collumn, yPos_StartPos + row, drawTextureInWindowList [i].width /* 70 */, drawTextureInWindowList [i].height /* 70 */), drawTextureInWindowList [i]))
            {
                foreach (KeyValuePair<int, Sprite> pair in ImageLibrary.spriteDictionary)
                {
                    if (drawTextureInWindowList [i].name.ToLower() == pair.Value.name.ToLower())
                    {
                        grid.selectedTileInteger = pair.Key;
                        grid.selectedTileName = pair.Value.name;
                    }
                }
            }
        }
    }
    
    private void SaveEditorWindowSettings()
    {
        File.Create(editorDataLocation).Close();
        
        string editorData = "";
        TextWriter myTextWriter = new StreamWriter(editorDataLocation);
        
        for (int i = 0; i < grid.ListOfAddedCategoryStrings.Count; i++)
        {
            rowAndTagList.Add(grid.ListOfAddedCategoryStrings [i].ToString());
            for (int k = 0; k < grid.ListOfAllSubStringLists[i].Count; k++)
            {
                rowAndTagList.Add(i + grid.ListOfAllSubStringLists [i] [k].ToString());
            }
        }
        
        for (int m = 0; m < rowAndTagList.Count; m++)
        {
            editorData += rowAndTagList [m].ToString();
            if (m < rowAndTagList.Count - 1)
            {
                editorData += ",";
            }
        }
        
        myTextWriter.Write(editorData);   
        myTextWriter.Close();
    }
    
    private void LoadEditorWindowSettings()
    {
        StreamReader myStreamReader = new StreamReader(editorDataLocation);
        string tagData = myStreamReader.ReadToEnd();
        myStreamReader.Close();
        
        if (System.String.IsNullOrEmpty(tagData))
        {
            return;
        }
        
        string[] tags = tagData.Split(',');
        
        for (int i = 0; i < tags.Length; i++)
        {   
            //Check if the first character of the tags[i] string is a number.
            Match match = Regex.Match(tags [i], @"^[0-9]");
            
            //If match is not success, that means its a Row.
            if (!match.Success)
            {
                bool rowBool = false;
                
                grid.ListOfAddedCategoryBools.Add(rowBool);
                grid.ListOfAddedCategoryStrings.Add(tags [i].ToString());
                
                List<string> addedSubStringList = new List<string>();
                List<bool> addedSubBoolList = new List<bool>();
                grid.ListOfAllSubStringLists.Add(addedSubStringList);
                grid.ListOfAllSubBoolLists.Add(addedSubBoolList);
                
                //If match succes, that means its a Tag.
            } else
            {
                
                for (int j = 0; j < grid.ListOfAddedCategoryStrings.Count; j++)
                {
                    if (int.Parse(match.Value) == j)
                    {
                        bool tagBool = false;
                        
                        grid.ListOfAllSubBoolLists [j].Add(tagBool);
                        
                        //Substring removes the first character from the tag string, so that the list int identifier wont be displayed in the editor window.
                        string tag = tags [i].ToString().Substring(1);
                        grid.ListOfAllSubStringLists [j].Add(tag);
                    }
                }
            }
        }
    }
    
    private void CloseWindowOnEnteringPlayMode()
    {
        EditorApplication.playmodeStateChanged = () =>
        {
            if (EditorApplication.isPlayingOrWillChangePlaymode && !EditorApplication.isPlaying)
            {
                this.Close(); //Whenever Close() is called, OnDestroy() is automatically called.
            }  
        };   
    }
    
    private void DeleteAll()
    {
        grid.ListOfAddedCategoryBools.Clear();
        grid.ListOfAddedCategoryStrings.Clear();
        grid.ListOfAllSubStringLists.Clear();
        grid.ListOfAllSubBoolLists.Clear();
        temporaryKeywordSpriteList.Clear();
        drawTextureInWindowList.Clear();
        grid.selectedWindowTab = "";
    }
    
    private void OnDestroy()
    {
        ImageLibrary.DestroySpriteAndTextureData();
        grid.editMode = false;
        
        SaveEditorWindowSettings();
        DeleteAll();
        
        InspectorLockToggle.ToggleInspectorLock();
    }
}

