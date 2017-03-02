Surface::Surface()
{
    pimpl=new impl;
}
Surface::Surface(const Surface& inc) : Surface()
{
    *pimpl=*(inc.pimpl);
}
Surface& Surface::operator = (const Surface& inc)
{
    *pimpl=*(inc.pimpl);
    return *this;
}
Surface::Surface(Surface&& inc)
{
    pimpl=inc.pimpl;
    inc.pimpl=nullptr;
}
Surface& Surface::operator = (Surface&& inc)
{
    *(pimpl)=*(inc.pimpl);
    return *this;
}
Surface::~Surface()
{
    delete pimpl;
}
