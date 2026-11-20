#ifndef LRASSET_HPP
#define LRASSET_HPP

namespace LR {
	class Asset {
	public:
		Asset(const char* const Path);
		~Asset();


	private:
		const char* const m_Path;
	};
}

#endif