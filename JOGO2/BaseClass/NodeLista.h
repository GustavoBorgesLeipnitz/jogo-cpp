#pragma once
#include <Windows.h>

template <typename ValueType>

class NodeLista {
public:
	NodeLista* next;
	ValueType value;
	NodeLista() {
		this->next = nullptr;
	};
	NodeLista(ValueType value) {
		this->value = value;
		this->next = nullptr;
	};
};