/* (C) Copyright 2013 Rob Watson rmawatson [at] hotmail.com  and others.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Contributors:
 *     Rob Watson ( rmawatson [at] hotmail )
 *
 */


#pragma once


#include <vector>
#include <boost/cstdint.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>


namespace PyMT4
{

typedef char byte;
typedef std::vector<byte> Buffer;
typedef std::vector<byte>::const_iterator BufferCPos;
typedef std::vector<byte>::iterator BufferPos;

template <typename T,typename Y> 
struct std::remove_const< std::pair< T, Y > >
{
	typedef  std::pair< typename std::remove_const<T>::type, typename std::remove_const<Y>::type> type;
};


template <typename T>
struct is_serializeable
{
	template <class U, void  (U::*)(Buffer&) const > 
	struct has_serialize{ };

	template <typename U>
	static boost::type_traits::yes_type& has_serialize_helper(has_serialize<U,&U::serialize>*);

	template <typename>
	static boost::type_traits::no_type& has_serialize_helper(...);

	BOOST_STATIC_CONSTANT(bool,value = sizeof(has_serialize_helper<T>(0)) == sizeof(boost::type_traits::yes_type));
};

template<typename T> struct is_iterable
{ 

	template <typename U> 
	static boost::type_traits::yes_type& has_begin_helper(void*, typename U::iterator b = ((U*)0)->begin(), typename U::iterator e = ((U*)0)->end());

	template <typename> 
	static boost::type_traits::no_type& has_begin_helper(...);

	BOOST_STATIC_CONSTANT(int32_t,value = sizeof(has_begin_helper<T>(0)) == sizeof(boost::type_traits::yes_type));
};

template <typename T>
struct is_insertable
{

	struct InsertB 
	{
		typedef bool value_type;
		struct const_iterator{};
		struct iterator{};
		iterator insert(const_iterator,value_type){}
	};

	struct Insertable : public T, public InsertB {};

	template <typename Y, Y t>  class InsertableHelper{};

	template <typename U>
	static boost::type_traits::no_type& has_insert_helper(U*, InsertableHelper<typename InsertB::iterator (InsertB::*)(typename InsertB::const_iterator,
		typename InsertB::value_type),
		&U::insert>* = 0);

	static boost::type_traits::yes_type& has_insert_helper(...);

	BOOST_STATIC_CONSTANT(bool,value = sizeof(has_insert_helper((Insertable*)(0))) == sizeof(boost::type_traits::yes_type) );
};

template<typename T> struct is_resizeable
{ 

	template <class U, void  (U::*)(size_t,typename U::value_type) > 
	struct has_resize{ };

	template <typename U>
	static boost::type_traits::yes_type& has_resize_helper(has_resize<U,&U::resize>*);

	template <typename>
	static boost::type_traits::no_type& has_resize_helper(...);

	BOOST_STATIC_CONSTANT(bool,value = sizeof(has_resize_helper<T>(0)) == sizeof(boost::type_traits::yes_type));
};

template<typename T> struct is_reserveable
{ 

	template <class U, void  (U::*)(size_t) > 
	struct has_reserve{ };

	template <typename U> static boost::type_traits::yes_type& has_reserve_helper(has_reserve<U,&U::reserve>*);

	template <typename> static boost::type_traits::no_type& has_reserve_helper(...);

	BOOST_STATIC_CONSTANT(bool,value = sizeof(has_reserve_helper<T>(0)) == sizeof(boost::type_traits::yes_type));
};

template<typename T> struct is_pair
{ 
	template <typename U> 
	static boost::type_traits::yes_type& has_first_second_helper(void*, typename U::first_type f=U::first_type(),typename U::second_type s=U::second_type());

	template <typename> static boost::type_traits::no_type& has_first_second_helper(...);

	BOOST_STATIC_CONSTANT(bool,value = sizeof(has_first_second_helper<T>(0)) == sizeof(boost::type_traits::yes_type));
};

template <typename T,typename U> struct SerializeHelper;

template <typename T> struct Serializer
{
	template <typename T,typename U> friend struct SerializeHelper;

	static void serializeItem(T* serializable,std::vector<byte>* buffer)
	{
		SerializeHelper<typename std::remove_const<T>::type >::serializeType(const_cast< typename std::remove_const<T>::type* >(serializable),buffer);
	}

	static void deserializeItem(T* destination,std::vector<byte>::const_iterator* bufferIter)
	{
		SerializeHelper<typename std::remove_const<T>::type >::deserializeType(destination,bufferIter);
	}

	static void serializeIterable(T* serializable,std::vector<byte>* buffer)
	{
		boost::uint32_t size = serializable->size();
		Serializer<boost::uint32_t>::serializeSimple(&size,buffer);

		for(T::iterator iter = serializable->begin(); iter != serializable->end();iter++)
		{
			SerializeHelper<T::value_type>::serializeType(&(*iter),buffer);
		}
	}

	static void serializePair(T* serializable,std::vector<byte>* buffer)
	{
		SerializeHelper<std::remove_const<T::first_type>::type>::serializeType(&serializable->first,buffer);
		SerializeHelper<T::second_type>::serializeType(&serializable->second,buffer);
	}

	static void serializeSimple(T* serializable,std::vector<byte>* buffer)
	{
		const byte * incInter,* copyIter;
		std::vector<byte>::iterator bufferIter;
		buffer->resize(buffer->size() + sizeof(*serializable));
		bufferIter = buffer->end();
		std::advance(bufferIter,-((int)sizeof(T)));
		bool begin = bufferIter == buffer->begin();
		copyIter = reinterpret_cast<byte const*>(serializable);
		incInter = copyIter+(sizeof(T));
		std::copy(copyIter,incInter,bufferIter);
	}


	template <bool contains_pod> // false
	static inline void deserializeIterable(T* deserializable,std::vector<byte>::const_iterator* bufferIter )
	{
		const boost::uint32_t size = *reinterpret_cast<const boost::uint32_t*>(&(**bufferIter));
		*bufferIter +=  sizeof(boost::uint32_t);
		ReisizeHelper<T>::resize(deserializable,size);

		for (T::iterator iter = deserializable->begin();iter!= deserializable->end();++iter)
		{
			SerializeHelper<T::value_type>::deserializeType(&(*iter),bufferIter);
		}
	}

	template <>
	static inline void deserializeIterable<true>(T* deserializable,std::vector<byte>::const_iterator* bufferIter )
	{
		const boost::uint32_t size = *reinterpret_cast<const boost::uint32_t*>(&(**bufferIter));
		*bufferIter +=  sizeof(boost::uint32_t);
		ReisizeHelper<T>::resize(deserializable,size);

		(*deserializable).assign((*bufferIter),(*bufferIter)+size);
		*bufferIter += size;
	}

	static inline void deserializeSimple(T* deserializable,std::vector<byte>::const_iterator* bufferIter)
	{
		*deserializable = *reinterpret_cast<const T*>(&(**bufferIter));
		*bufferIter += sizeof(T);
	}

	static inline void deserializePair(T* deserializable,std::vector<byte>::iterator* bufferIter)
	{
		SerializeHelper<T::first_type>::deserializeType(const_cast<T::first_type*>(&deserializable->first),bufferIter);
		SerializeHelper<T::second_type>::deserializeType(&deserializable->second,bufferIter);
	}

};

template <typename T,typename Enable=void> 
struct ReisizeHelper
{ static inline void resize(T* containerType,boost::uint32_t size) {} };

template <typename T> 
struct ReisizeHelper <T,typename boost::enable_if<is_resizeable<T> >::type>
{
	static inline void resize(T* containerType,boost::uint32_t size)
	{ containerType->resize(size); }
};


template <typename T,typename Enable=void> 
struct ReserveHelper
{ static inline void reserve(T* containerType,boost::uint32_t size) {} };

template <typename T> 
struct ReserveHelper <T,typename boost::enable_if<is_reserveable<T> >::type>
{
	static inline void reserve(T* containerType,boost::uint32_t size)
	{ containerType->reserve(size); }
};



template <typename T,typename Enable=void> 
struct SerializeHelper
{
	typedef T U;

	static void serializeType(T* simpleType,std::vector<byte>* buffer)
	{
		Serializer<T>::serializeSimple(simpleType,buffer);
	}

	static void deserializeType(T* simpleType,std::vector<byte>::const_iterator* bufferIter)
	{
		Serializer<T>::deserializeSimple(simpleType,bufferIter);
	}
};


template <typename T> 
struct SerializeHelper <T,typename boost::enable_if<is_iterable<T> >::type>
{
	typedef typename std::remove_const<T>::type U;

	static void serializeType(const T* containerType,std::vector<byte>* buffer)
	{
		Serializer<typename std::remove_const<T>::type >::serializeIterable(const_cast< std::remove_const<T>::type*>(containerType),buffer);
	}

	static void deserializeType(T* containerType,std::vector<byte>::const_iterator* bufferIter)
	{
		Serializer<typename std::remove_const<T>::type >::deserializeIterable< boost::is_pod<typename T::value_type>::type::value >(const_cast< std::remove_const<T>::type*>(containerType),bufferIter);
	}

};

template <typename T> 
struct SerializeHelper <T,typename boost::enable_if<is_pair< typename std::remove_const<T>::type> >::type>
{
	typedef T U;

	static void serializeType(T* pairType,std::vector<byte>* buffer)
	{
		Serializer<T>::serializePair(pairType,buffer);
	}

	static void deserializeType(T* pairType,std::vector<byte>::const_iterator* bufferIter)
	{
		Serializer<T>::deserializePair(pairType,bufferIter);
	}

};

template <typename T> 
struct SerializeHelper <T,typename boost::enable_if<is_serializeable<typename std::remove_const<typename T>::type > >::type>
{
	typedef T U;

	static void serializeType(const T* serializeableType,std::vector<byte>* buffer)
	{
		serializeableType->serialize(*buffer);
	}

	static void deserializeType(T* serializeableType,std::vector<byte>::const_iterator* bufferIter)
	{
		serializeableType->deserialize(*bufferIter);
	}

};

}