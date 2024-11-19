#pragma once
#ifndef PYLIKE_NUMPY_H
#define PYLIKE_NUMPY_H

#include <iostream>  
#include <vector>  
#include <algorithm>  


namespace np
{
    namespace impl_
    {
        // 定义一个结构体，用于存储值和索引
        template <class T>
        struct IndexedValue {  
            T value;  
            int index;  
        
            // 构造函数  
            IndexedValue(T val, int idx) : value(val), index(idx) {}  
        
            // 比较函数，用于升序排序  
            bool operator<(const IndexedValue& other) const {  
                return value < other.value;  
            }  
        
            // 如果需要降序排序，可以定义一个比较函数或者重载>操作符  
        };  
        
        // 自定义比较函数，用于降序排序
        template <class T> 
        bool compareDesc(const IndexedValue<T>& a, const IndexedValue<T>& b) {  
            return a.value > b.value;  
        }
    }
      
    
    // 实现类似np.argsort的函数
    template <class T>
    std::vector<int> argsort(const std::vector<T>& vec, bool reverse = false) {  
        std::vector<impl_::IndexedValue<T>> indexedValues;  
    
        // 填充索引和值  
        for (size_t i = 0; i < vec.size(); ++i) {  
            indexedValues.emplace_back(vec[i], static_cast<int>(i));  
        }  
    
        // 根据值排序索引  
        if (reverse) {  
            std::sort(indexedValues.begin(), indexedValues.end(), impl_::compareDesc<T>);  
        } else {  
            std::sort(indexedValues.begin(), indexedValues.end());  
        }  
    
        // 提取索引  
        std::vector<int> indices;  
        for (const auto& iv : indexedValues) {  
            indices.push_back(iv.index);  
        }  
    
        return indices;  
    }
}

#endif