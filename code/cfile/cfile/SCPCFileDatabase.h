#pragma once
#include <memory>

template<typename ElementType>
class SCPObjectTable
{
	uint32_t NextId = 0;
	std::map<uint32_t, std::unique_ptr<ElementType>> InternalStorage;

	tl::optional<ElementType&> GetIfValid(uint32_t ObjectID) 
	{ 
		ElementType* Element = InternalStorage[ObjectID].get();
		if (Element != nullptr)
		{
			return *Element;
		}
		else
		{
			return {};
		}
	}
	
	void Erase(uint32_t ObjectID)
	{
		ElementType* Element = InternalStorage[ObjectID].get();
		if (Element != nullptr) 
		{
			InternalStorage[ObjectID].reset();
		}
	}

	const std::map<uint32_t, ElementType&> Filter(std::function<bool(ElementType&)> Predicate)
	{
		std::map<uint32_t, ElementType&> FilterResults;

		for (auto Pair : InternalStorage)
		{
			auto MaybeElement = GetIfValid(Pair.first);
			if (MaybeElement.has_value())
			{
				if (Predicate(*MaybeElement))
				{
					FilterResults[Pair.first] = *MaybeElement;
				}
			}
		}
		return FilterResults;
	}

	uint32_t Insert(ElementType&& Element) { NextId++; InternalStorage[NextId] = std::make_unique(std::move(Element)); return NextId; }
};