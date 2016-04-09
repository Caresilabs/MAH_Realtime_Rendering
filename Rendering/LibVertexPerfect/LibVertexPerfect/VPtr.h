#pragma once

template<typename T>
class VPtr {
public:

	VPtr() : Object( nullptr ) {
		References = new int( 1 );
	}

	VPtr( T* rhs ) : Object( rhs ) {
		References = new int( 1 );
	}

	VPtr( const VPtr<T>& rhs ) : References( rhs.References ), Object( rhs.Object ) {
		(*References)++;
	}

	VPtr<T>& operator=( const VPtr<T>& rhs ) {
		if ( this != &rhs ) {
			(*References)--;
			if ( *References == 0 ) {
				delete References;
				delete Object;
			}

			References = rhs.References;
			Object = rhs.Object;
			(*References)++;
		}
		return *this;
	}

	T& operator*() const {
		return *Object;
	}

	T* operator->() const {
		return Object;
	}

	operator T*() const {
		return Object;
	}

	operator T&() const {
		return *Object;
	}

	~VPtr() {
		(*References)--;

		if ( *References == 0 ) {
			delete References;
			delete Object;
		}
	}
private:
	T* Object;

	int* References;
};