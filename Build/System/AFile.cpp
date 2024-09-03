//#include "AFile.h"
//#include<DxLib.h>
//#include<cassert>
//AFile::AFile(const TCHAR* path)
//{
//	m_cursor = 0;
//	int size = FileRead_size(path);
//	m_handle = FileRead_open(path);
//	m_data.resize(size);
//	FileRead_read(m_data.data(), size, m_handle);
//}
//
//bool AFile::IsReadble() const
//{
//	if (m_data.empty())return true;
//	auto result = CheckHandleASyncLoad(m_handle);
//	assert(result != -1);
//	return !result;
//}
//
//bool AFile::Read(void* address, int size)
//{
//	if (!IsReadble())return false;
//	std::copy_n(&m_data[m_cursor], size, static_cast<uint8_t*>(address));
//	m_cursor += size;
//	return true;
//}
//
//void AFile::Close()
//{
//	m_data.clear();
//}
