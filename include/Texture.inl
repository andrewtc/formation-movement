namespace atc
{
	inline int Texture::getWidth() const
	{
		return m_width;
	}


	inline int Texture::getHeight() const
	{
		return m_height;
	}


	inline float Texture::getAspectRatio() const
	{
		return m_aspectRatio;
	}


	inline bool Texture::hasAlpha() const
	{
		return m_hasAlpha;
	}
}