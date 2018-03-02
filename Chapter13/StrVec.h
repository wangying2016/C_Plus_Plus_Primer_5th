// �� vector ���ڴ������Եļ�ʵ��
class StrVec {
public:
    StrVec(): // allocator ��Ա����Ĭ�ϳ�ʼ��
        elements(nullptr), first_free(nullptr), cap(nullptr) { }
    StrVec(const StrVec&);              // �������캯��
    StrVec(StrVec&&) noexcept;          // �ƶ����캯��
    StrVec &operator=(const StrVec&);   // ������ֵ�����
    StrVec &operator=(StrVec &&) noexcept; // �ƶ���ֵ�����
    ~StrVec();                          // ��������
    void push_back(const std::string&); // ����Ԫ��
    size_t size() const { return first_free - elements; }
    size_t capacity() const { return cap - elements; }
    std::string *begin() const { return elements; }
    std::string *end() const { return first_free; }
    // ...
private:
    Static std::allocator<std::string> alloc;  // ����Ԫ��
    // ������Ԫ�صĺ�����ʹ��
    void chk_n_alloc()
        { if (size() == capacity()) reallocate(); }
    // ���ߺ��������������캯������ֵ�����������������ʹ��
    std::pair<std::string*, std::string*> alloc_n_copy
        (const std::string*, const std::string*);
    void free();            // ����Ԫ�ز��ͷ��ڴ�
    void reallocate();      // ��ø����ڴ沢��������Ԫ��
    std::string *elements;  // ָ��������Ԫ�ص�ָ��
    std::string *first_free;// ָ�������һ������Ԫ�ص�ָ��
    std::string *cap;       // ָ������β��λ�õ�ָ��
};

void StrVec::push_back(const string& s)
{
    chk_n_alloc();  // ȷ���пռ�������Ԫ��
    // �� first_free ָ���Ԫ���й��� s �ĸ���
    alloc.construct(frist_free++, s);
}

part<string*, string*>
StrVec::alloc_n_copy(const string *b, const string *e)
{
    // ����ռ䱣�������Χ�е�Ԫ��
    auto data = alloc.allocate(e - b);
    // ��ʼ��������һ�� pair���� pair �� data �� uninitialzed_copy �ķ���ֵ����
    return {data, uninitialzed_copy(b, e, data)};
}

void StrVec::free()
{
    // ���ܴ��ݸ� deallocate һ����ָ�룬��� elements Ϊ 0������ʲôҲ����
    if (elemetns) {
        // �������پ�Ԫ��
        for (auto p = first_free; p != elements; /* �� */)
            alloc.destroy(--p);
        alloc.deallocate(elements, cap - elements);
    }
}

StrVec::StrVec(const StrVec &s)
{
    // ���� alloc_n_copy ����ռ��������� s ��һ�����Ԫ��
    auto newdata = alloc_n_copy(s.begin(), s.end());
    elements = newdata.first;
    first_free = cap = newdata.second;
}

StrVec::~StrVec() 
{
    free();
}

StrVec &StrVec::operator=(const StrVec &rhs)
{
    // ���� alloc_n_copy �����ڴ棬��С�� rhs ��Ԫ��ռ�ÿռ�һ����
    auto data = alloc_n_copy(rhs.begin(), rhs.end());
    free();
    elements = data.first;
    first_free = cap = data.second;
    return *this;
}

void StrVec::reallocate()
{
    // ���ǽ����䵱ǰ��С�������ڴ�ռ�
    auto newcapacity = size() ? 2 * size() : 1;
    // �������ڴ�
    auto newdata = alloc.allocate(newcapacity);
    // �����ݴӾ��ڴ��ƶ������ڴ�
    auto dest = newdata;        // ָ������������һ������Ԫ��
    auto elem = elements;       // ָ���������һ��Ԫ��
    for (size_t i = 0; i != size(); ++i)
        alloc.construct(dest++, std::move(*elem++));
    free(); // һ�������ƶ���Ԫ�ؾ��ͷž��ڴ�ռ�
    // �������ǵ����ݽṹ��ִ����Ԫ��
    elements = newdata;
    first_free = dest;
    cap = elements + newcapacity;
}

StrVec::StrVec(StrVec &&s) noexcept // �ƶ�������Ӧ�׳��κ��쳣
    // ��Ա��ʼ�����ӹ� s �е���Դ
    : elements(s.elements), first_free(s.first_free), cap(s.cap)
{
    // �� s ����������״̬ -- �����������������ǰ�ȫ��
    s.elements = s.first_free = s.cap = nullptr;
}

// ʹ���ƶ�������
StrVec &StrVec::operator=(StrVec &&rhs) noexcept
{
    // ֱ�Ӽ���Ը�ֵ
    if (this != &rhs) {
        free();                 // �ͷ�����Ԫ��
        elements = rhs.elements;// �� rhs �ӹ���Դ
        frist_free = rhs.first_free;
        cap = rhs.cap;
        // �� rhs ���ڿ�����״̬
        rhs.elements = rhs.first_free = rhs.cap = nullptr;
    }
    return *this;
}

void StrVec::reallocate()
{
    // ���������ڵ�ǰ��ģ���ڴ�ռ�
    auto newcapacity = size() ? 2 * size() : 1;
    auto first = alloc.allocate(newcapacity);
    // �ƶ�Ԫ��
    auto last = uninitialized_copy(make_move_iterator(begin()),
                                   make_move_iterator(end()),
                                   first);
    // �ͷžɿռ�
    free();
    // ����ָ��
    elements = first;
    first_free = last;
    cap = elements + newcapacity;
}