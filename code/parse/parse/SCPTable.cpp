#include "SCPTable.h"
#include <memory>
#include <functional>
#include <string>

std::shared_ptr<SCPParsedTableData> FindFirstNode(const SCPParsedTableData& Parent, std::function<bool(std::shared_ptr<SCPParsedTableData>)> Predicate)
{
	for (auto Node : Parent.nodes)
	{
		if (Predicate(Node))
		{
			return Node;
		}
	}
	return std::shared_ptr<SCPParsedTableData>();
}

bool HasNodeWithName(const SCPParsedTableData& Parent, std::string Name, bool WholeExactMatch /*= true*/)
{
	for (auto Node : Parent.nodes)
	{
		if (WholeExactMatch ? Node->name == Name : Node->name.find(Name) != std::string::npos)
		{
			return true;
		}
	}
	return false;
}
