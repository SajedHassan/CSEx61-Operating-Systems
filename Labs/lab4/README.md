# Distributed Redlock Implementation Project

## **Project Overview**
This project involves implementing the **Redlock Algorithm** for distributed locks in a simulated distributed environment. The goal is to synchronize access to a shared resource (e.g., a counter) in a fault-tolerant and reliable way.

Using Docker, you will set up a multi-node Redis environment and implement the Redlock Algorithm to manage distributed locks. By completing this project, you will gain practical experience in distributed synchronization, fault tolerance, and containerized environments.

---

## **Learning Objectives**
1. Understand the concept of distributed locks and their importance in distributed systems.
2. Learn the **Redlock Algorithm** for reliable synchronization.
3. Gain hands-on experience with **Redis** and **Docker Compose**.
4. Develop and test a fault-tolerant system in a simulated environment.

---

## **Problem Statement**
Implement a **distributed locking mechanism** using the Redlock Algorithm to control access to a shared resource in a simulated distributed system. 

### **Requirements**
1. Simulate a multi-node Redis cluster using **Docker Compose** (5 Redis nodes).
2. Implement the **Redlock Algorithm** in Python (or another programming language of choice).
3. Control access to a shared resource (e.g. Incrementing a counter stored in a file).
4. Ensure fault tolerance:
   - The system must operate correctly even if up to **two Redis nodes** fail.
5. Use logging to record lock acquisition, release, and failed attempts.

---

### **Dependencies**
Create a virtual environment
```bash
python3 -m venv venv
source venv/bin/activate
```

Install the required Python libraries using `pip`:
```bash
pip install -r requirements.txt
```

----
## **System Requirements**

### **Software**
1. **Docker** and **Docker Compose**
   - Ensure Docker is installed on your system. Follow [Docker Installation Guide](https://docs.docker.com/get-docker/) if needed.
2. **Redis**
   - Redis instances will be set up using Docker containers.

---

## **Getting Started**

### **2. Set Up Docker Environment**
Use the provided `docker-compose.yml` file to start 5 Redis instances:
```bash
docker-compose up
```
This will create a multi-node Redis environment with the following nodes:
- `redis-node-1`
- `redis-node-2`
- `redis-node-3`
- `redis-node-4`
- `redis-node-5`

### **3. Implement the Redlock Algorithm**
- Write the logic to acquire and release locks across the Redis nodes.
- Use `SET NX PX` commands to implement locking and ensure a unique identifier is used for each lock.

### **4. Run the Application**
Execute the Python script that implements the Redlock Algorithm and demonstrates access to a shared resource:
```bash
python redlock_simulation.py
```

### **5. Test Fault Tolerance**
Simulate failures by stopping one or more Redis nodes:
```bash
docker stop redis-node-2 redis-node-3
```
Observe how the system continues to function with the remaining nodes.

---

## **Project Files**

### **1. `docker-compose.yml`**
Defines the Docker configuration for setting up 5 Redis instances.

### **2. `redlock_simulation.py`**
Python script implementing the Redlock Algorithm and simulating access to a shared resource.

### **3. `README.md`**
This documentation file explaining the project and requirements.

---

## **Expected Output**
1. **Logs of Lock Operations**:
   - Successful lock acquisition and release.
   - Failed attempts to acquire the lock.

   Example:
   ```
   Node 1: Acquiring lock...
   Node 1: Lock acquired.
   Node 1: Accessing critical section...
   Node 1: Releasing lock.
   ```

2. **Fault Tolerance Demonstration**:
   - The system continues to function when up to 2 Redis nodes fail.

---

## **Further Reading**
- [Redlock Algorithm](https://redis.io/docs/latest/develop/use/patterns/distributed-locks/)
- [Docker Documentation](https://docs.docker.com/get-started/)
- [Redis Commands: SETNX](https://redis.io/docs/latest/commands/setnx/)

---

### **Contact**
For any questions or clarifications, feel free to contact the teaching assistant (Eng. Sajed).
