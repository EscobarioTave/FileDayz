modded class MainMenu
{
	private ref VPPNewsFeed m_VPPNewsFeed;
	
	void MainMenu()
	{
		m_VPPNewsFeed = new VPPNewsFeed();
	}

	void ~MainMenu()
	{
		delete m_VPPNewsFeed;
	}
	
	override Widget Init()
	{
		layoutRoot = super.Init();
		if (m_VPPNewsFeed.ContentLoaded())
		{
			m_VPPNewsFeed.ConstructNewsFeed(layoutRoot);
		}
		return layoutRoot;
	}

	override void Update(float timeslice)
	{
		super.Update( timeslice );
		if (!m_VPPNewsFeed.m_UIInit && m_VPPNewsFeed.ContentLoaded())
		{
			m_VPPNewsFeed.ConstructNewsFeed(layoutRoot);
		}
	}
};