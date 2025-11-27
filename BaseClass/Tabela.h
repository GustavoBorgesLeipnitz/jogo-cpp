#pragma once
#include "NodeTabela.h"
#include <Windows.h>
#include <iostream>



template <typename KeyType, typename ValueType>

class Tabela {
public:

	NodeTabela<KeyType,ValueType>* head = nullptr;
	NodeTabela<KeyType,ValueType>* tail = nullptr;
	int size = 0;

	Tabela() {};

	void Add(KeyType key, ValueType value) {
		if (!head) {
			head = new NodeTabela<KeyType, ValueType>(key, value);
			tail = head;
		} else {
			tail->next = new NodeTabela<KeyType, ValueType>(key, value);
			tail = tail->next;
		}
		size++;
	}


	ValueType Get(KeyType key) {
		NodeTabela<KeyType, ValueType>* next = head;
		while (next) {
			if (next->key == key) {
				return next->value;
			}
			next = next->next;
		}
		return NULL;
	}

	KeyType* GetKeys() {
		KeyType* keys = new KeyType[size];
		NodeTabela<KeyType, ValueType>* next = head;
		int count = 0;
		while (next) {
			keys[count] = next->key;
			next = next->next;
			count++;
		}
		return keys;
	}

	ValueType* GetValues() {
		ValueType* values = new ValueType[size];
		NodeTabela<KeyType, ValueType>* next = head;
		int count = 0;
		while (next) {
			values[count] = next->value;
			next = next->next;
			count++;
		}
		return values;
	}

	void Set(KeyType key, ValueType value) {
		NodeTabela<KeyType, ValueType>* next = head;
		while (next) {
			if (next->key == key) {
				next->value = value;
			}
			next = next->next;
		}
	}


	void Remove(KeyType key) {
		NodeTabela<KeyType, ValueType>* anterior = nullptr;
		NodeTabela<KeyType, ValueType>* next = head;
		while (next) {
			if (next->key == key) {
				if (next == head) {
					if (head == tail) {
						delete head;
						head = nullptr;
						tail = head;
					} else {
						NodeTabela<KeyType, ValueType>* temp = head;
						head = head->next;
						delete temp;
					}
				}
				else if (next == tail) {
					anterior->next = nullptr;
					delete next;
					tail = anterior;
				}
				else {
					anterior->next = next->next;
					delete next;
				}
				size--;
				return;
			}
			anterior = next;
			next = next->next;
		}
	}

	void Remove(ValueType value) {
		NodeTabela<KeyType, ValueType>* anterior = nullptr;
		NodeTabela<KeyType, ValueType>* next = head;
		while (next) {
			if (next->value == value) {
				if (next == head) {
					if (head == tail) {
						delete head;
						head = nullptr;
						tail = head;
					}
					else {
						NodeTabela<KeyType, ValueType>* temp = head;
						head = head->next;
						delete temp;
					}
				}
				else if (next == tail) {
					anterior->next = nullptr;
					delete next;
					tail = anterior;
				}
				else {
					anterior->next = next->next;
					delete next;
				}
				size--;
				return;
			}
			anterior = next;
			next = next->next;
		}
		std::cout << "N ACHO PRA DELETA\n";
	}


};