
#ifndef BINARY_FILE_HANDLER_H
#define BINARY_FILE_HANDLER_H
#pragma once

#include "pilotfile/FileHandler.h"

#include "globalincs/pstypes.h"
#include "cfile/cfile.h"

namespace pilot {
class BinaryFileHandler: public FileHandler {
	CFILE* _cfp;

	struct SectionOffset {
		Section id;
		size_t offset;
	};

	SCP_vector<SectionOffset> _sectionOffsets;


	size_t _section_start_pos = INVALID_SIZE;
	size_t _section_end_pos = INVALID_SIZE;

	bool _in_array = false;
 public:
	explicit BinaryFileHandler(CFILE* cfp);

	~BinaryFileHandler() override;

	void writeUByte(const char* name, std::uint8_t value) override;

	void writeShort(const char* name, std::int16_t value) override;

	void writeInt(const char* name, std::int32_t value) override;

	void writeUInt(const char* name, std::uint32_t value) override;

	void writeFloat(const char* name, float value) override;

	void writeString(const char* name, const char* str) override;


	void startSectionWrite(Section id) override;

	void endSectionWrite() override;


	void startArrayWrite(const char* name, size_t size, bool short_length = false) override;

	void endArrayWrite() override;


	virtual void flush() override;



	virtual std::uint8_t readUByte(const char* name);

	virtual std::int16_t readShort(const char* name);

	virtual std::int32_t readInt(const char* name);

	virtual std::uint32_t readUInt(const char* name);

	virtual float readFloat(const char* name);

	virtual SCP_string readString(const char* name);

	virtual void readString(const char* name, char* dest, size_t max_size);

	virtual void beginSectionRead();

	virtual bool hasMoreSections();

	virtual Section nextSection();

	virtual size_t startArrayRead(const char* name, bool short_index);

	virtual void endArrayRead();
};
}

#endif // BINARY_FILE_HANDLER_H
