# Access Card System — System Design Document

## 1. System Overview

The Access Card System is a software system designed to manage employee access cards in an office building.

The system is responsible for:

- managing employees,
- managing access cards,
- controlling access permissions,
- recording access history,
- storing system data.

The main goal of the system is to provide controlled access to the building while maintaining a complete history of card usage.

Employee and card records remain stored in ordered vectors, with hash maps from IDs to vector indexes for fast lookup. Lookup returns an optional reference wrapper: callers can use a record when found without handling nullable raw pointers. Employee IDs use the `E` prefix and six digits; card IDs use `C` and six digits. Each system instance advances its next ID based on IDs added to it. IDs start at `000001`; after `999999`, no further ID of that type can be generated.

---

# 2. System Architecture

The system consists of four main components:
