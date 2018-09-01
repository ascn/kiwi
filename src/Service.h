#ifndef __SERVICE_H__
#define __SERVICE_H__

namespace Kiwi {

class Service {
private:
	Service() = delete;
	virtual ~Service() = 0;

public:
	static void Initialize() {}

};

}

#endif // __SERVICE_H__
