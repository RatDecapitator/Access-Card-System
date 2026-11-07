Employee Management
- Register employee
- Automatically assign the next employee ID in the `E000001` format
- Edit employee
- Remove employee
- Activate/deactivate employee

Card Management
- Register card
- Automatically assign the next card ID in the `C000001` format
- Assign card to employee
- Remove card assignment
- Transfer card to another employee
- Activate/deactivate card
- Report card as lost
- Mark card as broken

Search
- Search employee
- Search card
- Find employees and cards by ID using hash-based indexes
- Filter cards by status

History
- Record every important event

Persistence
- Save all data automatically

Business Rules
- Only active cards assigned to active employees may access the building.
- Generated IDs advance past matching IDs already added to the current in-memory system.