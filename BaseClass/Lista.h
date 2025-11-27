#pragma once
#include <Windows.h>
#include <iostream>
#include "NodeLista.h"



template <typename ValueType>
class Lista {
public:
	NodeLista<ValueType>* head = nullptr;
	NodeLista<ValueType>* tail = nullptr;
	NodeLista<ValueType>* ultimo_acessado = nullptr;
	int ultimo_acessado_index = -5;
	int size = 0;

	void Add(ValueType value) {
		if (!head) {
			head = new NodeLista<ValueType>(value);
			tail = head;
		} else {
			tail->next = new NodeLista<ValueType>(value);
			tail = tail->next;
		}
		size++;
	}

	ValueType Get(int index) {
		if (index < 0 || index >= size)
			throw std::out_of_range("Índice fora dos limites");

		

		int count = 0;

		NodeLista<ValueType>* next = head;


		if (index >= ultimo_acessado_index && ultimo_acessado_index != -5) {
			count = ultimo_acessado_index;
			next = ultimo_acessado;
		}

		while (next) {
			if (count == index) {
				ultimo_acessado_index = count;
				ultimo_acessado = next;
				return next->value;
			}
			next = next->next;
			count++;
		}
	}

	ValueType* GetValues() {
		ValueType* values = new ValueType[size];

		int count = 0;
		NodeLista<ValueType>* next = head;

		while (next) {
			values[count] = next->value;
			next = next->next;
			count++;
		}

		return values;
	}

	void Set(ValueType value, int index) {
		if (index < 0 || index > size)
			return;

		if (index == size) {
			Add(value);
			return;
		}

		int count = 0;

		NodeLista<ValueType>* next = head;

		while (next) {
			if (count == index) {
				next->value = value;
				return;
			}
			next = next->next;
			count++;
		}

	}

	void Remove(int index) {
		if (index < 0 || index >= size)
			return;


		if (index == 0) {
			if (size == 1) {
				delete head;
				head = nullptr;
				tail = nullptr;
			}
			else {
				NodeLista<ValueType>* temp = head;
				head = head->next;
				delete temp;
			}
			ultimo_acessado_index = -5;
			size--;
			return;
		}

		
		int count = 1;
		NodeLista<ValueType>* anterior = head;
		NodeLista<ValueType>* next = head->next;

		while (next) {
			if (count == index) {
				anterior->next = next->next;
				delete next;
				if (count == size - 1) {
					tail = anterior;
				}
				ultimo_acessado_index = -5;
				size--;
				return;
			}
			anterior = next;
			next = next->next;
			count++;
		}


	}

	void Remove(ValueType value) {
		int count = 0;
		NodeLista<ValueType>* anterior = nullptr;
		NodeLista<ValueType>* next = head;
		while (next) {
			if (next->value == value) {
				this->Remove(count);
			}
			next = next->next;
			count++;
		}
		
	}


};