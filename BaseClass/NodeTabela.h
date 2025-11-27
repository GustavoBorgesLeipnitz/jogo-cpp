#pragma once

#include <Windows.h>

template <typename KeyType, typename ValueType>

class NodeTabela {
public:
	NodeTabela* next;
	KeyType key;
	ValueType value;
	NodeTabela(KeyType key, ValueType value) {
		this->key = key;
		this->value = value;
		this->next = nullptr;
	};
};