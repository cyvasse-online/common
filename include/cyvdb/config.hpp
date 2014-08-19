#ifndef _DB_CONFIG_HPP_
#define _DB_CONFIG_HPP_

#include <string>

namespace cyvdb
{
	class DBConfig
	{
		private:
			std::string _matchDataUrl;

			DBConfig()
			{ }

			DBConfig(const DBConfig&) = delete;
			const DBConfig& operator= (const DBConfig&) = delete;

		public:
			static DBConfig& glob()
			{
				static DBConfig _glob;
				return _glob;
			}

			const std::string& getMatchDataUrl() const
			{
				return _matchDataUrl;
			}

			void setMatchDataUrl(const std::string& url)
			{
				_matchDataUrl = url;
			}
	};
}

#endif // _DB_CONFIG_HPP_
