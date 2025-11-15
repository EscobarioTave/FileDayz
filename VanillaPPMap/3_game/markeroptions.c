class MarkerIcon {
    private string iconName;
    private string iconPath;

    void MarkerIcon(string name, string path) {
        iconName = name;
        iconPath = path;
    }

    string getMarkerIconName() {
        return iconName;
    }

    string getMarkerIconPath() {
        return iconPath;
    }
};

class MarkerColor {
    private string colorName;
    private	vector colorVector;

    void MarkerColor(string name, vector _vector) {
        colorName   = name;
        colorVector = _vector;
    }

    string getColorName() {
        return colorName;
    }

    vector getColorVector() {
        return colorVector;
    }
};

class MarkerOptions {

    protected ref array<ref MarkerIcon>  m_MarkerIcons;
    protected ref array<ref MarkerColor> m_MarkerColors;

    static ref MarkerOptions s_instance;

    private void MarkerOptions() {
        m_MarkerIcons = new array<ref MarkerIcon>;
        m_MarkerColors = new array<ref MarkerColor>;

        m_MarkerIcons.Insert( new MarkerIcon("Circle","VanillaPPMap\\GUI\\Textures\\CustomMapIcons\\waypointeditor_CA.paa") );
        m_MarkerIcons.Insert( new MarkerIcon("Waypoint","VanillaPPMap\\GUI\\Textures\\CustomMapIcons\\waypoint_CA.paa") );
        m_MarkerIcons.Insert( new MarkerIcon("WaypointCompleted","VanillaPPMap\\GUI\\Textures\\CustomMapIcons\\waypointcompleted_CA.paa") );
        m_MarkerIcons.Insert( new MarkerIcon("Ruin","dz\\gear\\navigation\\data\\map_ruin_ca.paa") );
        m_MarkerIcons.Insert( new MarkerIcon("House","VanillaPPMap\\GUI\\Textures\\CustomMapIcons\\house.paa") );
        m_MarkerIcons.Insert( new MarkerIcon("Tent","VanillaPPMap\\GUI\\Textures\\CustomMapIcons\\tent.paa") );
        m_MarkerIcons.Insert( new MarkerIcon("Car","VanillaPPMap\\GUI\\Textures\\CustomMapIcons\\car.paa") );
        m_MarkerIcons.Insert( new MarkerIcon("Bordercross","dz\\gear\\navigation\\data\\map_border_cross_ca.paa") );
        m_MarkerIcons.Insert( new MarkerIcon("Broadleaf","dz\\gear\\navigation\\data\\map_broadleaf_ca.paa") );
        m_MarkerIcons.Insert( new MarkerIcon("Church","dz\\gear\\navigation\\data\\map_church_ca.paa") );
        m_MarkerIcons.Insert( new MarkerIcon("Fir","dz\\gear\\navigation\\data\\map_fir_ca.paa") );
        m_MarkerIcons.Insert( new MarkerIcon("Hill","dz\\gear\\navigation\\data\\map_hill_ca.paa") );
        m_MarkerIcons.Insert( new MarkerIcon("Viewpoint","dz\\gear\\navigation\\data\\map_viewpoint_ca.paa") );

        m_MarkerColors.Insert( new MarkerColor("White",Vector(255,255,255)) );
        m_MarkerColors.Insert( new MarkerColor("Red",Vector(200,0,0)) );
        m_MarkerColors.Insert( new MarkerColor("Orange",Vector(255,165,0)) );
        m_MarkerColors.Insert( new MarkerColor("Yellow",Vector(255,255,0)) );
        m_MarkerColors.Insert( new MarkerColor("Green",Vector(0,200,0)) );
        m_MarkerColors.Insert( new MarkerColor("Cyan",Vector(0,255,255)) );
        m_MarkerColors.Insert( new MarkerColor("Blue",Vector(0,0,200)) );
        m_MarkerColors.Insert( new MarkerColor("Purple",Vector(128,0,128)) );
    }

    static ref MarkerOptions GetInstance() {
        if(!s_instance) {
            s_instance = new MarkerOptions;
        }
        return s_instance;
    }

    /*
    	Returns all icon objects
    */
    ref array<ref MarkerIcon> GetAllIcons() {
        return m_MarkerIcons;
    }

    /*
    	Returns all color objects
    */
    ref array<ref MarkerColor> GetAllColors() {
        return m_MarkerColors;
    }
};