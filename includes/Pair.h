#pragma once
#include <stdexcept>

template<typename K, typename S>
class Pair
{
public:
	Pair()
	{
		hasKey_ = false;
		hasData_ = false;
	}

	Pair(K key, S data)
	{
		this->key_ = key;
		this->data_ = data;
		hasKey_ = true;
		hasData_ = true;
	}

	bool hasKey()
	{
		return hasKey_;
	}

	bool hasData()
	{
		return hasData_;
	}

	K getKey()
	{
		if (!hasKey())
		{
			throw std::logic_error("undefined key");
		}

		return key_;
	}

	S getData()
	{
		if (!hasData())
		{
			throw std::logic_error("undefined data");
		}

		return data_;
	}

	void setKey(K key)
	{
		key_ = key;
		hasKey_ = true;
	}

	void setData(S data)
	{
		data_ = data;
		hasData_ = true;
	}

	bool operator==(const Pair<K, S>& other)
	{
		if ((hasKey_ != other.hasKey_) || (hasData_ != other.hasData_))
		{
			return false;
		}

		if (hasKey_ && key_ != other.key_)
		{
			return false;
		}

		return hasData_ && (data_ == other.data_);
	}

	bool operator!=(const Pair<K, S>& other)
	{
		return !operator==(other);
	}

private:
	K key_;
	S data_;
	bool hasKey_;
	bool hasData_;
};
