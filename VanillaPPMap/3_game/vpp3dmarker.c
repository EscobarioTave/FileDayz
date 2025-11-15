class VPP3DMarker extends UIScriptedMenu {
    private Widget           m_MarkerRootWidget;
    private GridSpacerWidget m_SpacerGrid;
    private ImageWidget      m_MarkerIcon;
    private TextWidget       m_MarkerNameWidget;
    private TextWidget       m_MarkerDistance;

    private string           m_MarkerName;
    private string           m_iconPath;
    private	vector	         m_MarkerPositon;
    private vector           m_MarkerPosPLANE;
    private bool             m_IsMarkerVisible;

    void VPP3DMarker( string MarkerName, string iconPath, vector MarkerPos, bool visible ) {
        m_MarkerRootWidget = GetGame().GetWorkspace().CreateWidgets( "VanillaPPMap/GUI/Layouts/3DMarker.layout" );
        m_SpacerGrid       = GridSpacerWidget.Cast( m_MarkerRootWidget.FindAnyWidget( "SpacerGrid" ) );

        m_MarkerIcon       = ImageWidget.Cast( m_MarkerRootWidget.FindAnyWidget( "MarkerIcon" ) );
        m_MarkerNameWidget = TextWidget.Cast( m_SpacerGrid.FindAnyWidget( "MarkerName" ) );
        m_MarkerDistance   = TextWidget.Cast( m_SpacerGrid.FindAnyWidget( "MarkerDistance" ) );

        m_MarkerPosPLANE   = MarkerPos;
        m_MarkerName  	   = MarkerName;
        m_iconPath 	       = iconPath;
        vector SnappedPos  = SnapToGround( Vector(MarkerPos[0], MarkerPos[1], MarkerPos[2]) );
        m_MarkerPositon    = Vector(SnappedPos[0], SnappedPos[1] + 1, SnappedPos[2]);
        m_IsMarkerVisible  = visible;

        m_MarkerIcon.LoadImageFile(0,m_iconPath);
        m_MarkerNameWidget.SetText( m_MarkerName );
        m_SpacerGrid.Update();
        m_MarkerNameWidget.Update();
        m_MarkerDistance.Update();

        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.DoUpdate, 1, true);
    }

    void ~VPP3DMarker() {
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.DoUpdate);
        if (m_MarkerRootWidget != NULL) m_MarkerRootWidget.Show( false );
    }

    float CalcDistance() {
        return vector.Distance( GetGame().GetPlayer().GetPosition(), m_MarkerPositon );
    }

    vector Get3DMarkerPosition() {
        return m_MarkerPosPLANE;
    }

    void DoUpdate() {
        if (IsMarkerVisible() && m_MarkerRootWidget != NULL && GetGame().GetPlayer()) {
            vector ScreenPosRelative = GetGame().GetScreenPosRelative(m_MarkerPositon);
            if( ScreenPosRelative[0] >= 1 || ScreenPosRelative[0] == 0 || ScreenPosRelative[1] >= 1 || ScreenPosRelative[1] == 0 ) {
                m_MarkerRootWidget.Show( false );
                return;
            } else if( ScreenPosRelative[2] < 0 ) {
                m_MarkerRootWidget.Show( false );
                return;
            } else {
                m_MarkerRootWidget.Show( true );
            }

            float pos_x,pos_y;

            vector ScreenPos = GetGame().GetScreenPos(m_MarkerPositon);
            pos_x = ScreenPos[0];
            pos_y = ScreenPos[1];

            pos_x = Math.Ceil(pos_x);
            pos_y = Math.Ceil(pos_y);

            m_MarkerRootWidget.SetPos(pos_x,pos_y);
            m_MarkerDistance.SetText( Math.Floor(CalcDistance()).ToString() + "m" );

            m_SpacerGrid.Update();
            m_MarkerNameWidget.Update();
            m_MarkerDistance.Update();
        } else if (m_MarkerRootWidget != NULL) {
            m_MarkerRootWidget.Show(false);
        }
    }

    bool IsMarkerVisible() {
        return m_IsMarkerVisible;
    }

    void SetMarkerVisible(bool visible) {
        /*if (!g_Game.CanUse3DMarkers() && visible)
        {
            m_IsMarkerVisible = false;
            return;
        }*/
        m_IsMarkerVisible = visible;
    }


    /**************************************
    /********** Utility methods /**********
    /**************************************

    /*
    	Convert string to vector format
    */
    static vector StringToVector(string str) {
        vector oV = "0 0 0";
        TStringArray strs = new TStringArray;
        str.Replace("<","");
        str.Replace(">","");
        str.Replace(","," ");

        str.Split( " ", strs );

        oV = Vector(strs.Get(0).ToFloat(),strs.Get(1).ToFloat(),strs.Get(2).ToFloat());
        return oV;
    }

    /*
    	Convert vector to string format
    */
    static string VectorToString(vector vctr) {
        string oStr;
        oStr = vctr[0].ToString();
        oStr += "," + vctr[1].ToString();
        oStr += "," + vctr[2].ToString();
        return oStr;
    }

    static vector SnapToGround(vector pos) {
        float pos_x = pos[0];
        float pos_z = pos[2];
        float pos_y = GetGame().SurfaceY(pos_x, pos_z);
        vector tmp_pos = Vector(pos_x, pos_y, pos_z);
        tmp_pos[1] = tmp_pos[1] + pos[1];

        return tmp_pos;
    }
}