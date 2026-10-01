# Adaptive Campus Mobility & Parking System

<img width="1600" height="1200" alt="WhatsApp Image 2026-10-01 at 01 11 54" src="https://github.com/user-attachments/assets/f14e2db7-fe10-43f2-947d-968b72ce7a83" />



DEMO VIDEO:https://youtu.be/qYMLHrs8pRw



An Arduino-based smart parking guidance and campus mobility system designed to improve parking navigation, reduce unnecessary vehicle movement, and provide an adaptive foundation for future IoT-enabled parking management.

## 🚗 Project Overview

The current prototype demonstrates a basic smart parking guidance system using an **Arduino Uno, ultrasonic sensors, LED indicators, and MAX7219 8×8 displays**.

The system detects parking availability and guides incoming vehicles between **ground-level and basement parking** based on real-time occupancy.

The prototype currently demonstrates the concept using **one ground-level slot and one basement slot**, while providing a foundation for scaling toward a multi-slot intelligent parking system.

## ⚙️ Current Prototype

The prototype can:

* Detect vehicle presence using **HC-SR04 ultrasonic sensors**
* Monitor parking-slot occupancy
* Indicate slot status using **Red, Green and Yellow LEDs**
* Display directional guidance using **MAX7219 8×8 LED matrices**
* Guide vehicles toward available parking areas
* Prioritize ground-level parking before basement parking
* Display **G (Ground), B (Basement), or X (Stop)** at the entry point

### Parking Status

* 🟢 **Green** — Parking slot available
* 🟡 **Yellow** — Vehicle detected / incoming state
* 🔴 **Red** — Parking slot occupied

## 🔧 Hardware Used

* Arduino Uno
* HC-SR04 Ultrasonic Sensors ×3
* MAX7219 8×8 LED Matrix Displays ×3
* Red LEDs ×2
* Green LEDs ×2
* Yellow LEDs ×2
* Resistors
* Breadboards
* Jumper Wires

## 🧠 Proposed Future Development

The current prototype can be evolved into a **Reservation-Aware Adaptive Parking Guidance System**.

The proposed system would support multiple parking slots such as:

**Ground:** G1, G2, G3, ...
**Basement:** B1, B2, B3, ...

### 1. Advance Reservation

A driver could provide:

* Vehicle number
* Estimated arrival time / arrival window
* Preferred parking requirement, if applicable

The system would temporarily reserve a suitable parking slot for the expected arrival period.

### 2. Vehicle Verification

At the parking entrance, a camera-based **number-plate recognition system** could identify the incoming vehicle.

If the detected vehicle has a valid reservation:

**Vehicle Identification → Reservation Verification → Reserved Slot → Navigation**

The system would guide the vehicle directly toward its assigned reserved slot.

### 3. Adaptive Allocation for Non-Reserved Vehicles

If the vehicle has **no valid reservation**, the system would automatically switch to real-time parking allocation.

It would:

1. Check available ground-level slots such as **G1, G2, G3, ...**
2. Select the **closest available ground-level slot**
3. If all ground-level slots are occupied, check basement slots such as **B1, B2, B3, ...**
4. Select the **closest available basement slot**
5. Provide dynamic guidance to the assigned slot

### 4. Reservation Expiry

Reservations would be associated with an expected arrival window.

If a reserved vehicle does not arrive within the permitted time window, the reservation could expire and the slot could be released for other vehicles.

### 5. IoT & Cloud Integration

Future development could include:

* Real-time occupancy monitoring
* Cloud/database connectivity
* Reservation management
* Remote parking-status monitoring
* Web/mobile interface
* Parking usage analytics

## 🔬 Proposed Research Direction

Rather than treating reservation, vehicle identification, or sensor-based parking as isolated features, the proposed direction is to investigate their **combined use in adaptive parking allocation**.

A potential research question is:

> **Can reservation-aware and vehicle-verified dynamic slot allocation reduce unnecessary parking search and improve parking-slot utilization compared with conventional real-time parking guidance?**

The future system can therefore be evaluated using measurable parameters such as:

* Parking search time
* Vehicle travel distance inside the parking area
* Slot utilization
* Reservation fulfillment rate
* Allocation accuracy
* Waiting time

## 🏗️ Development Roadmap

**Current Prototype**
Arduino + Sensors + Displays
↓
**Multi-Slot Parking**
G1/G2/G3 + B1/B2/B3
↓
**Real-Time Occupancy & Allocation**
↓
**Reservation System**
↓
**Vehicle Number-Plate Verification**
↓
**Reservation-Aware Adaptive Guidance**
↓
**IoT / Cloud Integration & Performance Evaluation**

## 🏆 Project

Developed for **GENOVATE 2.0**

**Team:** InvertedFLIP_FLOPS

**Project:** Adaptive Campus Mobility & Parking System

**Team Achievement:** 2nd Rank GENOVATE 2.0
---
Git workflow setup completed.
