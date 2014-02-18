using UnityEngine;
using System.Collections;
using System.Globalization;

[ExecuteInEditMode]
public class TileAligner : MonoBehaviour {

    Grid grid;

    void Awake() {
        grid = FindObjectOfType(typeof(Grid)) as Grid;
    }

    void Update() {
        float snapX = Mathf.Floor(this.gameObject.transform.position.x) + 0.5f;
        float snapY = Mathf.Floor(this.gameObject.transform.position.y) + 0.5f;
        float snapZ = Mathf.Floor(this.gameObject.transform.position.z);
        
        this.gameObject.transform.position = new Vector3(snapX, snapY, snapZ);
        grid.AttachNameToSprite(this.gameObject);
    }
}