class VPPNewsFeed: ScriptedWidgetEventHandler
{
	bool                            		m_UIInit;
	bool                                    IsShowing;
	private Widget 		 	 				m_VPPNewsFeed;
	private ScrollWidget 	 				m_Scroller_Content;
	private GridSpacerWidget 				m_ContentGrid;
	private ButtonWidget 				    vpp_news_feed_close;
	private ref array<ref VPPNewsElementUI> m_NewsElements;
	private ref VPPNewsFeedDataManager      m_NewsFeedData;

	void VPPNewsFeed()
	{
		m_NewsElements = new array<ref VPPNewsElementUI>;
		m_NewsFeedData = new VPPNewsFeedDataManager();
	}

	void ~VPPNewsFeed()
	{
		delete m_NewsFeedData;
		delete m_NewsElements;
	}

	void ConstructNewsFeed(Widget root, bool showHideBtn = false)
	{
		if (root == null)
		{
			Print("[V++ Map] Failed to ConstructNewsFeed()");
			return;
		}

		if (IsShowing && m_UIInit)
		{
			ToggleNewsFeed(false);
			return;
		}

		m_UIInit  = true;
		m_VPPNewsFeed 	   = GetGame().GetWorkspace().CreateWidgets("VanillaPPMap/GUI/Layouts/VPPNewsFeed.layout", root);
		m_Scroller_Content = ScrollWidget.Cast(m_VPPNewsFeed.FindAnyWidget("Scroller_Content"));
		m_ContentGrid  	   = GridSpacerWidget.Cast(m_VPPNewsFeed.FindAnyWidget("ContentGrid"));
		vpp_news_feed_close = ButtonWidget.Cast(m_VPPNewsFeed.FindAnyWidget("vpp_news_feed_close"));
		vpp_news_feed_close.Show(showHideBtn);
		
		array<ref VPPNewsElement> newsElements = m_NewsFeedData.GetElements();
		foreach(ref VPPNewsElement e : newsElements)
		{
			m_NewsElements.Insert(new VPPNewsElementUI(m_ContentGrid, e, showHideBtn));
		}
		IsShowing = true;
		m_VPPNewsFeed.SetHandler(this);
		m_VPPNewsFeed.SetSort(1023);
	}

	override bool OnClick(Widget w, int x, int y, int button)
    {
    	if (w == vpp_news_feed_close)
    	{
    		ToggleNewsFeed(false);
    		return true;
    	}
    	return false;
    }

    void ShiftPosition()
    {
    	//For use ingame
    	float x,y
    	m_VPPNewsFeed.GetPos(x, y);
    	m_VPPNewsFeed.SetPos(x + 750.0, y + 8.0);
    }

	void SetContentLoaded(bool state)
	{
		m_NewsFeedData.Setloaded(state);
	}

	void ToggleNewsFeed(bool state)
	{
		IsShowing = state;
		m_VPPNewsFeed.Show(state);
	}

	bool ContentLoaded()
	{
		return m_NewsFeedData.ContentLoaded();
	}

	ref VPPNewsFeedDataManager GetDataManager()
	{
		return m_NewsFeedData;
	}
};