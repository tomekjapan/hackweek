#ifndef ROBOCOM_CLIENT_HANDLE_H
#define ROBOCOM_CLIENT_HANDLE_H

namespace robocom {
namespace client
{

	typedef int SysHandleType;

	class Handle
	{
	public:

		Handle () throw ();
		Handle (SysHandleType handle) throw ();
		~Handle () throw ();

		bool isOpen () const throw ();
		SysHandleType getNative () const throw ();

		void assign (SysHandleType handle) throw ();
		void close () throw ();

	private:
		Handle (const Handle& other);
		Handle& operator= (const Handle& other);

		SysHandleType m_handle;
	};

} }

#endif // ROBOCOM_CLIENT_HANDLE_H
