#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>

using namespace std;

class DuplicateItemException : public std::runtime_error {
public:
    DuplicateItemException(const std::string& msg) : std::runtime_error(msg) {}
};

class ItemNotFoundException : public std::runtime_error {
public:
    ItemNotFoundException(const std::string& msg) : std::runtime_error(msg) {}
};

class StoredItem {
private:
    std::string id;
    std::string description;
    std::string location;

public:
    StoredItem(std::string id, std::string desc, std::string loc)
        : id(id), description(desc), location(loc) {}

    std::string getId() const { return id; }
    std::string getDescription() const { return description; }
    std::string getLocation() const { return location; }
};

class StorageManager {
private:
    std::unordered_map<std::string, std::shared_ptr<StoredItem> > itemById;
    std::map<std::string, std::shared_ptr<StoredItem> > itemByDescription;

public:
    void addItem(const std::shared_ptr<StoredItem>& item) {
        // TODO: Add item to both maps, throw if ID already exists
        cout << "Adding item: " << item->getId() << " - " << item->getDescription() << endl;
        const std::string& id = item->getId();
        if (itemById.count(id)) {
            throw DuplicateItemException("Item with ID " + id + " already exists!");
        }
        itemById[id] = item;
        itemByDescription[item->getDescription()] = item;
    }

    std::shared_ptr<StoredItem> findById(const std::string& id) const {
        // TODO: Return item if found or throw ItemNotFoundException
        cout << "Retrieving " << id << "...\n";
        auto it = itemById.find(id);
        if (it == itemById.end()) {
            throw ItemNotFoundException("Item not found: " + id);
        }
        return it->second;
    }

    void removeItem(const std::string& id) {
        // TODO: Remove from both maps, throw if not found
        cout << "Removing " + id + "...\n";
        auto it = itemById.find(id);
        if (it == itemById.end()) {
            throw ItemNotFoundException("Cannot remove non-existent item: " + id);
        }
        itemByDescription.erase(it->second->getDescription());
        itemById.erase(it);
    }

    void listItemsByDescription() const {
        // TODO: Iterate over itemByDescription and print info
        for (const auto& pair : itemByDescription) {
            const auto& item = pair.second;
            std::cout << "- " << item->getDescription() << ": " << item->getLocation() << endl;
        }
    }
};

void testDuplicateAddition(StorageManager& manager) {
    auto item = std::make_shared<StoredItem>("ITEM001", "LED Light", "Aisle 3, Shelf 1");
    try {
        manager.addItem(item);
    } catch (const DuplicateItemException& e) {
        std::cout << e.what() << "\n";
    }
}

void testItemNotFound(const StorageManager& manager) {
    try {
        manager.findById("ITEM003");
    } catch (const ItemNotFoundException& e) {
        std::cout << e.what() << "\n";
    }
}

int main() {
    // TODO: Create StorageManager and test functionality
    StorageManager manager;

    auto item1 = std::make_shared<StoredItem>("ITEM001", "LED Light", "Aisle 3, Shelf 1");
    auto item2 = std::make_shared<StoredItem>("ITEM002", "Fan Motor", "Aisle 2, Shelf 5");

    try {
        manager.addItem(item1);
        manager.addItem(item2);
        testDuplicateAddition(manager);

        auto found = manager.findById("ITEM002");
        std::cout << "Found: " << found->getDescription() << " at " << found->getLocation() << "\n";

        manager.removeItem("ITEM003");
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    testItemNotFound(manager);

    std::cout << "Items in description order:\n";
    manager.listItemsByDescription();
    return 0;
}