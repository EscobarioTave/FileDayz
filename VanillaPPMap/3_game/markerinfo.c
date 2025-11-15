class MarkerInfo {
    private string M_MARKER_NAME;
    private string M_ICON_PATH;
    private vector M_COLOR;
    private vector M_POSITION;
    private bool   M_ISACTIVE;
    private bool   M_IS_3D_ACTIVE;

    [NonSerialized()]
    private bool   m_3DForcedDisabled;

    void MarkerInfo(string name, string path, vector color, vector pos, bool isActive = true, bool is3DActive = false ) {
        M_MARKER_NAME  = name;
        M_ICON_PATH	   = path;
        M_COLOR		   = color;
        M_POSITION	   = pos;
        M_ISACTIVE     = isActive;
        M_IS_3D_ACTIVE = is3DActive;
    }

    void SetName(string newName) {
        M_MARKER_NAME = newName;
    }

    void SetIconPath(string newIcon) {
        M_ICON_PATH = newIcon;
    }

    void SetColor(vector newColor) {
        M_COLOR = newColor;
    }

    void SetPosition(vector newPos) {
        M_POSITION = newPos;
    }

    void SetActive(bool state) {
        M_ISACTIVE = state;
    }

    void Set3DActive(bool state) {
        M_IS_3D_ACTIVE = state;
    }

    void Set3DForcedDisabled(bool state){
        m_3DForcedDisabled = state;
    }

    string GetName() {
        return M_MARKER_NAME;
    }

    string GetIconPath() {
        return M_ICON_PATH;
    }

    vector GetColor() {
        return M_COLOR;
    }

    vector GetPosition() {
        return M_POSITION;
    }

    bool IsActive() {
        return M_ISACTIVE;
    }

    bool Is3DActive() {
        return M_IS_3D_ACTIVE;
    }

    bool Is3DForcedDisabled(){
        return m_3DForcedDisabled;
    }
};
