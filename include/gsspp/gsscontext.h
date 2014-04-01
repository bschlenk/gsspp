#ifndef __GSSCONTEXT_H__
#define __GSSCONTEXT_H__


#include <gssapi.h>
#include "gssmech.h"
#include "gsscredential.h"

class GSSBuffer;
class GSSName;

class GSSContext
{
 public:
	// constructors
	GSSContext() : _context( GSS_C_NO_CONTEXT ) {}
	GSSContext( const GSSBuffer& buff );

	// assignment operators
	GSSContext& operator= ( const GSSContext& other );

	~GSSContext() { clear(); }

	void clear();
	bool valid() { return _context != GSS_C_NO_CONTEXT; }

	gss_ctx_id_t       raw()       { return _context; }
	const gss_ctx_id_t raw() const { return _context; }

	operator       gss_ctx_id_t ()       { return raw();  }
	operator const gss_ctx_id_t () const { return raw();  }
	operator       gss_ctx_id_t*()       { return &_context; }
	operator const gss_ctx_id_t*() const { return &_context; }

	class Flags
	{
	 public:
		Flags() : _flags( 0 ) {}
		Flags( OM_uint32 f ) : _flags( f ) {}

		void set( OM_uint32 bit, bool enable = true ) { if ( enable ) _flags |= bit; else _flags &= ~bit; }
		bool check( OM_uint32 bit ) const { return _flags & bit; }

		bool deleg_flag   () const { return check( GSS_C_DELEG_FLAG    ); }
		bool mutual_flag  () const { return check( GSS_C_MUTUAL_FLAG   ); }
		bool replay_flag  () const { return check( GSS_C_REPLAY_FLAG   ); }
		bool sequence_flag() const { return check( GSS_C_SEQUENCE_FLAG ); }
		bool conf_flag    () const { return check( GSS_C_CONF_FLAG     ); }
		bool integ_flag   () const { return check( GSS_C_INTEG_FLAG    ); }
		bool anon_flag    () const { return check( GSS_C_ANON_FLAG     ); }

		void deleg_flag   ( bool enable ) { set( GSS_C_DELEG_FLAG,    enable ); }
		void mutual_flag  ( bool enable ) { set( GSS_C_MUTUAL_FLAG,   enable ); }
		void replay_flag  ( bool enable ) { set( GSS_C_REPLAY_FLAG,   enable ); }
		void sequence_flag( bool enable ) { set( GSS_C_SEQUENCE_FLAG, enable ); }
		void conf_flag    ( bool enable ) { set( GSS_C_CONF_FLAG,     enable ); }
		void integ_flag   ( bool enable ) { set( GSS_C_INTEG_FLAG,    enable ); }
		void anon_flag    ( bool enable ) { set( GSS_C_ANON_FLAG,     enable ); }

		operator OM_uint32() const { return _flags; }
		OM_uint32 flags()    const { return _flags; }

	 private:
		OM_uint32 _flags;
	};

	bool initialize(
		GSSBuffer& input,
		const GSSName& target,
		const Flags& flags,
		const GSSCredential& cred = GSS_C_NO_CREDENTIAL,
		GSSMech mech = GSS_C_NO_OID,
		OM_uint32 time = 0,
		gss_channel_bindings_t bindings = GSS_C_NO_CHANNEL_BINDINGS
	);

	bool accept(
		GSSBuffer& input,
		const GSSCredential& cred = GSS_C_NO_CREDENTIAL,
		gss_channel_bindings_t bindings = GSS_C_NO_CHANNEL_BINDINGS
	);

	void import_context( const GSSBuffer& buff );
	GSSBuffer export_context();

	GSSBuffer get_mic( const GSSBuffer& message, gss_qop_t qop = GSS_C_QOP_DEFAULT ) const;
	bool verify_mic( const GSSBuffer& message, const GSSBuffer& mic ) const;

	GSSBuffer wrap    ( const GSSBuffer& message, bool encrypt, gss_qop_t qop = GSS_C_QOP_DEFAULT ) const;
	void wrap_in_place( GSSBuffer& message,       bool encrypt, gss_qop_t qop = GSS_C_QOP_DEFAULT ) const;
	static GSSBuffer wrap    ( const GSSContext& context, const GSSBuffer& message, bool encrypt, gss_qop_t qop = GSS_C_QOP_DEFAULT );
	static void wrap_in_place( const GSSContext& context,       GSSBuffer& message, bool encrypt, gss_qop_t qop = GSS_C_QOP_DEFAULT );

	GSSBuffer unwrap    ( const GSSBuffer& wrapped, bool * conf_state = 0, gss_qop_t * qop_state = 0 ) const;
	void unwrap_in_place(       GSSBuffer& wrapped, bool * conf_state = 0, gss_qop_t * qop_state = 0 ) const;
	static GSSBuffer unwrap    ( const GSSContext& context, const GSSBuffer& message, bool * conf_state = 0, gss_qop_t * qop_state = 0 );
	static void unwrap_in_place( const GSSContext& context,       GSSBuffer& message, bool * conf_state = 0, gss_qop_t * qop_state = 0 );

	size_t wrap_size_limit( size_t max_size, bool encrypt, gss_qop_t qop = GSS_C_QOP_DEFAULT ) const;

 private:
	gss_ctx_id_t _context;

};


#endif // __GSSCONTEXT_H__
