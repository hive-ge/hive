namespace hive
{
    template <class T, class D> struct IndexedPointer {

      private:
        int i = -1;

      public:
        IndexedPointer(const int v) : i(v) {}

        IndexedPointer(const IndexedPointer<T, D> & ptr) : i(ptr.i)
        {
            D::incrementIndexedReference(i);
        }

        ~IndexedPointer() { D::decrementIndexedReference(i); }

        T & operator*() const { return *D::retrieveIndexedPointer(i); }

        T * operator->() { return D::retrieveIndexedPointer(i); }
    };
} // namespace hive