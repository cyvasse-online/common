#ifndef _CYVDB_CONFIG_HPP_
#define _CYVDB_CONFIG_HPP_

#include <string>

namespace cyvdb
{
	class DBConfig
	{
		private:
			std::string m_matchDataUrl;

			DBConfig()
			{ }

			DBConfig(const DBConfig&) = delete;
			const DBConfig& operator= (const DBConfig&) = delete;

		public:
			static DBConfig& glob()
			{
				static DBConfig s_glob;
				return s_glob;
			}

			const std::string& getMatchDataUrl() const
			{ return m_matchDataUrl; }

			void setMatchDataUrl(const std::string& url)
			{ m_matchDataUrl = url; }
	};
}

#endif // _CYVDB_CONFIG_HPP_
