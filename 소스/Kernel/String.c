#include "String.h"

UINTN
DUMMYAPI GetStringLength(
	IN CONST	BYTE*	_string
)	{
	UINTN   length = 0;

	for (length = 0; _string[length] != '\0'; ++length);

	return length;
}

VOID
DUMMYAPI CopyString(
	IN CONST	BYTE*	_source,
	OUT			BYTE*	_destination
)	{
	UINTN length = GetStringLength(_source);

	_destination[length] = '\0';

	while (--length > 0) {
		_destination[length] = _source[length];
	}

	_destination[0] = _source[0];
}

INT64
DUMMYAPI CompareString(
	IN CONST	BYTE*	_source1,
	IN CONST	BYTE*	_source2,
	IN CONST	UINTN	_length
)	{
	UINTN   index;

	for (index = 0; index < _length; ++index) {
		if (_source1[index] > _source2[index] OR _source2[index] == NUL) {
			return 1;
		}
		
		if (_source1[index] < _source2[index] OR _source1[index] == NUL) {
			return -1;
		}
	}

	return 0;
}

VOID
DUMMYAPI ConvertDecimalToString(
	OUT			BYTE*	_string,
	IN CONST	UINT64	_number,
	IN CONST	BOOLEAN	_sign
)	{
	UINT64	index = 0;
	UINT64	length = 0;
	UINTN	number = _number;

	BYTE	buffer[100] = {0, };

	if (number == 0) {
		_string[0] = '0';
		_string[1] = '\0';

		return ;
	}

	if (_sign AND _number > MAX_INT64) {
		buffer[0] = '-';
		number = -(INT64)number;
		++index;
	}

	while (number > 0) {
		buffer[index] = '0' + number % 10;
		number /= 10;
		++index;
	}

	--index;
	length = index;

	if (buffer[0] == '-') {
		_string[0] = '-';
		while (index > 0) {
			_string[length - index + 1] = buffer[index];
			--index;
		}
	} else {
		while (index > 0) {
			_string[length - index] = buffer[index];
			--index;
		}
		_string[length] = buffer[0];
	}

	_string[length + 1] = '\0';
}

//BYTE* FloatToString(BYTE* string, double number) {
//    
//}

VOID
DUMMYAPI ConvertHexadecimalToString(
	OUT			BYTE* _string,
	IN CONST	UINT64	_number,
	IN CONST	BOOLEAN _caps
)	{
	UINT64	index = 0;
	UINT64	length = 0;
	UINT64	number = _number;

	BYTE	buffer[100] = { 0, };

	if (number == 0) {
		_string[0] = '0';
		_string[1] = '\0';

		return;
	}

	while (number > 0) {
		if (_caps) {
			buffer[index] = (number % 16 >= 10) ? 'A' + (number % 16 - 10) : '0' + number % 16;
		} else {
			buffer[index] = (number % 16 >= 10) ? 'a' + (number % 16 - 10) : '0' + number % 16;
		}
		number /= 16;
		++index;
	}

	--index;
	length = index;

	while (index > 0) {
		_string[length - index] = buffer[index];
		--index;
	}

	_string[length] = buffer[0];
	_string[length + 1] = '\0';
}