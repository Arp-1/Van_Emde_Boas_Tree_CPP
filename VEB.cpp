#ifndef VAN_EBDE_BOAS_TREE_CPP

    #define VAN_EBDE_BOAS_TREE_CPP
    #include <math.h>
    #include <string>
    #include <vector>

    namespace std
    {
        template<typename T>
        class VEBTree{
            private:
                short universe_size;
                T min;
                T elements_count = 0;
                VEBTree* summary = NULL;
                VEBTree** cluster = NULL;
                long long bytes_used = sizeof(VEBTree<T>);

            public:
                VEBTree(short universe_size){
                    this->universe_size = universe_size;
                }
                T get_bytes_in_use(){
                    return this->bytes_used;
                }
                T size(){
                    return this->elements_count;
                }
                T length(){
                    return this->elements_count;
                }
                T find_minimum(){
                    if(!this->elements_count)
                        throw new string("The Tree is Empty");
                    else
                        return this->min;
                }
                void insert_element(T num);
                void delete_element(T num);
                bool search_element(T num);
                T find_successor_of(T num);
                T find_predecessor_of(T num);
                vector<T> get_all_elements(void);
                void get_all_elements_as_parameter(vector<T>& vec);
        };

        template<typename T>
        void VEBTree<T>::insert_element(T num){
            short w = (this->universe_size)/2;
            T u = 1 << (this->universe_size - w);

            if(!this->elements_count){
                this->min = num;
                this->cluster = new VEBTree*[u];
                for(T j = 0; j < u; j++){
                    this->cluster[j] = NULL;
                    this->bytes_used += sizeof(VEBTree*);
                }
            }
            else {

                if((this->min > num)){
                    swap(this->min, num);
                }

                T c = (num >> w) & (u - 1);

                if( !(this->cluster[c]) || !(this->cluster[c]->elements_count)){
                    if((!this->cluster[c])){
                        this->cluster[c] = new VEBTree(w);
                        this->bytes_used += this->cluster[c]->bytes_used;
                    }
                    if(!this->summary){
                        this->summary = new VEBTree(w);
                        this->bytes_used += this->summary->bytes_used;
                    }
                    this->bytes_used -= this->summary->bytes_used;
                    this->summary->insert_element(c);
                    this->bytes_used += this->summary->bytes_used;
                }

                this->bytes_used -= this->cluster[c]->bytes_used;
                this->cluster[c]->insert_element(num);
                this->bytes_used += this->cluster[c]->bytes_used;
            }
            this->elements_count++;
        }

        template<typename T>
        T VEBTree<T>::find_predecessor_of(T num){
            if((!this->elements_count) || (this->min >= num)){
                throw new string("This number has no predecessor");
            }
            else{
                if(this->summary){
                    if(this->summary->elements_count){
                        short w = (this->universe_size)/2;
                        T u = 1 << (this->universe_size - w);
                        T c = (num >> w) & (u - 1);

                        try{
                            if(this->cluster[c]){
                                return this->cluster[c]->find_predecessor_of(num);
                            }
                            else
                                throw new string("This number has no predecessor");
                        }
                        catch(string* err){
                            T succ;
                            if(!err->compare("This number has no predecessor")){
                                try{
                                    succ = this->summary->find_predecessor_of(c);
                                    return this->cluster[succ]->find_predecessor_of(num);
                                }
                                catch(string* error){
                                    return this->min;
                                }
                            }
                        }
                    }
                    else {
                        return this->min;
                    }
                }
                else {
                    return this->min;
                }
            }
        }

        template<typename T>
        T VEBTree<T>::find_successor_of(T num){
            if(!this->elements_count){
                throw new string("This number has no successor");
            }
            else if(this->min > num){
                return this->min;
            }
            else{
                if(this->summary){
                    if(this->summary->elements_count){
                        short w = (this->universe_size)/2;
                        T u = 1 << (this->universe_size - w);
                        T c = (num >> w) & (u - 1);

                        try{
                            if(this->cluster[c])
                                return this->cluster[c]->find_successor_of(num);
                            else
                                throw new string("This number has no successor");
                        }
                        catch(string* err){
                            T succ;
                            if(!err->compare("This number has no successor")){
                                try{
                                    succ = this->summary->find_successor_of(c);
                                    return this->cluster[succ]->find_successor_of(num);
                                }
                                catch(string* error){
                                    throw error;
                                }
                            }
                        }
                    }
                    else {
                        throw new string("This number has no successor");
                    }
                }
                else {
                    throw new string("This number has no successor");
                }
            }
        }

        template<typename T>
        vector<T> VEBTree<T>::get_all_elements(){
            vector<T> vec;
            if(!this->elements_count)
                return vec;
            else{
                vec.push_back(this->min);
                if(!this->summary);

                else if(!this->summary->elements_count);

                else {
                    vector<T> vec1, vec2;
                    vec1 = this->summary->get_all_elements();
                    for(int i = 0; i < vec1.size(); i++){
                        vec2 = this->cluster[vec1[i]]->get_all_elements();
                        vec.insert(vec.end(), vec2.begin(), vec2.end());
                    }
                }
                return vec;
            }
        }

        template<typename T>
        void VEBTree<T>::get_all_elements_as_parameter(vector<T>& vec){
            if(!this->elements_count)
                return ;
            else{
                vec.push_back(this->min);
                if(!this->summary);

                else if(!this->summary->elements_count);

                else {
                    vector<T> vec1; 
                    this->summary->get_all_elements_as_parameter(vec1);
                    for(int i = 0; i < vec1.size(); i++){
                        this->cluster[vec1[i]]->get_all_elements_as_parameter(vec);
                    }
                }
            }
        }

        template<typename T>
        bool VEBTree<T>::search_element(T num){
            if(!this->elements_count)
                return false;
            else{

                if(this->min == num){
                    return true;
                }
                else {
                    short w = (this->universe_size)/2;
                    T u = 1 << (this->universe_size - w);
                    T c = (num >> w) & (u - 1);

                    if(this->cluster[c]){
                        if(this->cluster[c]->elements_count){
                            return this->cluster[c]->search_element(num);
                        }
                        else
                            return false;
                    }
                    else
                        return false;
                }
            }
        }

        template<typename T>
        void VEBTree<T>::delete_element(T num){
            short w = (this->universe_size)/2;
            T u = 1 << (this->universe_size - w);

            T c = (num >> w) & (u - 1);
            if(!this->elements_count){
                return ;
            }
            else{
                if(this->min == num){
                    if(!(this->summary)){

                        for(T j = 0; j < u; j++){
                            delete[] this->cluster[j];
                        }

                        delete[] this->cluster;
                        this->cluster = NULL;
                        this->bytes_used = sizeof(VEBTree<T>);
                        this->elements_count--;
                    }
                    else{
                        this->min = this->cluster[this->summary->min]->min;
                        // Remove previous amounts of bytes and elements used by this cluster
                        this->bytes_used -= this->cluster[this->summary->min]->bytes_used;
                        this->elements_count -= this->cluster[this->summary->min]->elements_count;
                        // Delete the number from the cluster
                        this->cluster[this->summary->min]->delete_element(this->min);
                        // Add new amounts of bytes and elements used by this cluster
                        this->bytes_used += this->cluster[this->summary->min]->bytes_used;
                        this->elements_count += this->cluster[this->summary->min]->elements_count;
                        //cout << "number of elements: " << this->cluster[this->summary->min]->elements_count << endl;
                        if(!(this->cluster[this->summary->min]->elements_count)){

                            // Remove bytes used by this cluster since it's going to be deleted
                            this->bytes_used -= this->cluster[this->summary->min]->bytes_used;
                            delete[] this->cluster[this->summary->min];
                            this->cluster[this->summary->min] = NULL;

                            // Remove previous amounts of bytes used by this cluster
                            this->bytes_used -= this->summary->bytes_used;

                            this->summary->delete_element(this->summary->min);
                            // Add new amounts of bytes used by this cluster
                            this->bytes_used += this->summary->bytes_used;
                        }
                        if(!(this->summary->elements_count)){
                            // Remove bytes used by this cluster since it's going to be deleted
                            this->bytes_used -= this->summary->bytes_used;
                            delete[] this->summary;

                            this->summary = NULL;
                        }
                    }
                }
                else{

                    if(!(this->cluster[c]))
                        return;
                    else if(!(this->cluster[c]->elements_count))
                        return;

                    // Remove previous amounts of bytes and elements used by this cluster
                    this->bytes_used -= this->cluster[c]->bytes_used;
                    this->elements_count -= this->cluster[c]->elements_count;

                    this->cluster[c]->delete_element(num);

                    // Add new amounts of bytes and elements used by this cluster
                    this->bytes_used += this->cluster[c]->bytes_used;
                    this->elements_count += this->cluster[c]->elements_count;

                    if(!(this->cluster[c]->elements_count)){
                        // Remove previous amounts of bytes used by this cluster
                        this->bytes_used -= this->summary->bytes_used;
                        this->summary->delete_element(c);
                        // Add new amounts of bytes used by this cluster
                        this->bytes_used += this->summary->bytes_used;
                        // Remove bytes used by this cluster since it's going to be deleted
                        this->bytes_used -= this->cluster[c]->bytes_used;
                        delete[] this->cluster[c];

                        this->cluster[c] = NULL;
                    }
                    if(!(this->summary->elements_count)){
                        // Remove bytes used by this cluster since it's going to be deleted
                        this->bytes_used -= this->summary->bytes_used;
                        delete[] this->summary;

                        this->summary = NULL;
                    }
                }
            }
        }

    }

#endif    